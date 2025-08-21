这些测试用于快速核对**退出码**与程序行为约定：

- 进程退出码必须与 `StatusCode` 一致：
  - 0 → 找到解（STATUS_OK）
  - 3 → 无解（STATUS_ERR_UNSOLVABLE）
  - 1 → 输入错误（STATUS_ERR_INPUT）
  - 2 → 资源错误（STATUS_ERR_MEMORY）
  - 4 → 非法参数（STATUS_ERR_INVALID_ARGUMENT）

使用：

```bash
make           # 或 MODE=release make
chmod +x tests/run.sh
make test      # 执行所有样例并给出 PASS/FAIL
make run       # 用一个最小样例运行
```

输出内容（解的网格或错误说明）仅用于人工查看；脚本以退出码为准判断 PASS/FAIL。
