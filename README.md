# RISC-V Simulator OoOE

**out-of-order execution** ~~OoO~~

`to do`

- [ ] Tomasulo
- [ ] a brief introduction about Tomasulo in README
- [ ] and so on……

### 目前版本

Tomasulo:从入门到找不到门



- 流程图

![](\img\proc1.png)

- 基础类

```mermaid
graph TD;
	A[Instruction]-->fecth;
	A-->decode;
	B[Memory]-->B1[init read];
	B-->load;
	B-->store;
```

```mermaid
graph TD;
	C[Register]-->data;
	C-->pc;
	C-->output;
	D[Executor]-->D1[run/execute];
	D-->D2[memory access];
	D-->D3[write back];
```

- 流水模块

```mermaid
sequenceDiagram
IF->>ID: init
ID->>EXE: init
EXE->>MEM: init
MEM->>WB: init
Note over IF: inst.fetch
Note over ID: inst.decode
Note over EXE: exe.run
Note over MEM: exe.memoryaccess
Note over WB: exe.writeback
```

### Ver 1.0

> 未完成
>
> 无控制指令的Tomasulo algorithm