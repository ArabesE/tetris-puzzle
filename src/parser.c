#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "tetromino.h"

/* 在此文件中实现输入解析：
 *   - 读取宽高；
 *   - 逐个解析形状记号与可选计数；
 *   - 支持注释与空白；
 *   - 填充 TetrominoBag 结构并进行基本一致性校验。 */

#define LINE_MAX_LENGTH 4096

StatusCode parse_from_stream(FILE *in, int *out_w, int *out_h, TetrominoBag *out_bag)
{
    if (!in || !out_w || !out_h || !out_bag)
        return STATUS_ERR_INVALID_ARGUMENT;

    // init outputs
    for (size_t i = 0; i < TETRO_TYPE_COUNT; ++i)
        out_bag->counts[i] = 0;
    out_bag->total = 0;
    *out_w = 0;
    *out_h = 0;

    char line[LINE_MAX_LENGTH];
    int width = 0, height = 0;
    bool got_sizes = false;

    while (fgets(line, (int)sizeof line, in))
    {
        // strip inline comment starting with '#'
        char *hash = strchr(line, '#');
        if (hash)
            *hash = '\0';

        // skip leading spaces
        char *p = line;
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n' || *p == '\v' || *p == '\f')
            ++p;
        if (*p == '\0')
            continue; // blank line

        if (!got_sizes)
        {
            // first non-empty, non-comment line must be: two positive integers (width height)
            int nconsumed = 0;
            int w = 0, h = 0;
            if (sscanf(p, " %d %d %n", &w, &h, &nconsumed) != 2)
                return STATUS_ERR_INPUT;
            // ensure nothing but whitespace after the two integers
            for (char *q = p + nconsumed; *q; ++q)
            {
                if (!(*q == ' ' || *q == '\t' || *q == '\r' || *q == '\n' || *q == '\v' || *q == '\f'))
                {
                    return STATUS_ERR_INPUT;
                }
            }
            if (w <= 0 || h <= 0)
                return STATUS_ERR_INPUT;
            width = w;
            height = h;
            got_sizes = true;
            continue;
        }

        // after sizes: parse piece tokens until EOF. Tokens are whitespace-separated.
        while (*p)
        {
            // skip spaces between tokens
            while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n' || *p == '\v' || *p == '\f')
                ++p;
            if (*p == '\0')
                break;

            // token must begin with a letter (shape)
            unsigned char ch = (unsigned char)*p;
            if (!isalpha(ch))
                return STATUS_ERR_INPUT;

            // map letter to type (case-insensitive)
            char tok[2];
            tok[0] = (char)toupper(ch);
            tok[1] = '\0';
            TetrominoType t = tetro_type_from_token(tok);
            if (t == TETRO_TYPE_COUNT)
                return STATUS_ERR_INPUT;
            ++p;

            // optional "*<count>" with count as positive integer, no spaces inside
            size_t count = 1;
            if (*p == '*')
            {
                ++p;
                // parse unsigned decimal > 0, manual to avoid extra headers
                size_t val = 0;
                if (!isdigit((unsigned char)*p))
                    return STATUS_ERR_INPUT;
                while (isdigit((unsigned char)*p))
                {
                    size_t digit = (size_t)(*p - '0');
                    // overflow check: val = val*10 + digit
                    if (val > (SIZE_MAX - digit) / 10)
                        return STATUS_ERR_INPUT;
                    val = val * 10 + digit;
                    ++p;
                }
                if (val == 0)
                    return STATUS_ERR_INPUT;
                count = val;
            }

            // accumulate into bag with overflow checks
            if (out_bag->counts[t] > SIZE_MAX - count)
                return STATUS_ERR_INPUT;
            out_bag->counts[t] += count;
            if (out_bag->total > SIZE_MAX - count)
                return STATUS_ERR_INPUT;
            out_bag->total += count;

            // loop continues to next token (whitespace or end-of-line)
        }
    }

    if (!got_sizes)
        return STATUS_ERR_INPUT;

    *out_w = width;
    *out_h = height;

    return STATUS_OK;
}

void parser_print_usage(FILE *out)
{
    if (!out)
        out = stderr;

    fprintf(out,
            "Usage: %s [INPUT_FILE]\n"
            "\n"
            "Input format:\n"
            "  First line: two positive integers <width> <height>\n"
            "  Following lines: tetromino list (case-insensitive), tokens separated by whitespace\n"
            "    - Single letters: I O T S Z J L   (each means one piece)\n"
            "    - Or counted form: I*3  t*2       ('*' followed by a positive integer)\n"
            "  '#' starts a comment to end of line; blank lines are ignored; read until EOF.\n"
            "  Parsing does not verify tileability (area divisibility is not required).\n"
            "\n"
            "Example:\n"
            "  8 5\n"
            "  I*2 O*2 T*2 S Z J L\n",
            PROJECT_NAME);
}