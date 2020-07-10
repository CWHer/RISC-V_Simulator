# RISC-V Simulator

`to do`

- [x] fix reg[0] bug
- [x] fix execute bug/how execute jal&jalr...
- [x] fix program counter/when to execute next_pc?
- [x] refactor EXE&MEM&WB
- [x] separate instructions executing section
- [x] mem access
- [x] writeback
- [x] MEM needs 3 clock
- [x] 5-stage pipeline
- [x] data hazard: forwarding
- [ ] EXE->IF forwarding
- [x] control hazard: prediction&api   
- [ ] optimize prediction
- [ ] optimize code runtime
- [ ] Tomasulo
- [ ] precise stop (?
- [ ] a brief introduction about RISC-V in README
- [ ] and so on……

### 目前版本

Ver3.0：两位饱和计数器分支预测的并行版本

~~现在跑得很慢，很慢....~~

- 流程图

![](\img\proc.png)

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

> 串行简易版本
>
> 主要实现了reg,mem,inst,exe这4个类

### Ver 1.1

> 串行版本
>
> 在原来的基础上添加了IF,ID,EXE,MEM,WB这5个模块
>
> 优化了执行流程，每一个模块可以嵌入不同的指令

### Ver 2.0

> 简易并行版本，~~效率比较低，遇事不决直接stall~~
>
> feature：
>
> - 增加了wait_clk变量，用来lock模块（putwclk=add wait clk)
>
> - 增加了reset，用于初始化模块
> - 非l&s inst直接传入WB，不经过MEM
>
> 某一个模块被lock时
>
> - 不会run，且每个周期wait_clk-1
>
> - 传出到下一个模块为空，且不会传入上一个模块的结果（对于Ver2.0情况，被lock时上一个模块应该一直为空）
>
> 会引发lock的情况
>
> - ID为control inst，e.g. JAL	结果：IF.reset()，IF.wait_clk=3，即pipeline暂停到cur inst执行完
> - ID为l&s inst，e.g. SB	结果：IF.reset()，IF.wait_clk=6，即pipeline暂停到cur inst执行完
>   - MEM为l&s inst	结果：MEM.wait_clk=3，模拟MEM需要3 clk

### Ver 2.1

> fix：
>
> - ID为l&s inst，只需IF.wait_clk=4，即cur inst完成WB后nxt inst刚好EXE

### Ver 2.2

> feature：
>
> - Forwarding 
>
>   i.e. MEM结果提早返回给EXE
>
>   ID为l&s inst，IF.wait_clk=3

### Ver 2.3

> 修复一个WAW data hazard
>
> 由于非s&l不经过MEM，可能同时发生MEM->WB&EXE->WB
>
> 当遇到这种情况时，暂停IF/ID/EXE至下一个clk

### ~~Ver 2.4~~废弃的版本

> > 原话：一个正常写的 .c 编译出来的东西，就不应该会出现这种情况啊
>
> 修复了s inst的一个bug
>
> bug会导致IF先于MEM
>
> 当遇到这种情况时，暂停IF至MEM完成
>
> ~~虽然测试数据里不存在这种bug~~
>
> ~~因为需要魔改代码，于是放弃了forwarding~~

### Ver 3.0

> feature：
>
> - 分支预测
>
>   两位饱和计数器的分支预测
>
>   正确率在40%~70%，平均在55%，~~有时候还不如全部not jump~~

### Ver 3.1

> feature
>
> - 两级自适应预测器
>
> ~~To be continued~~
>
> ~~现在代码跑得很慢，很慢...~~
>
> ~~代码性能问题暂时放一放~~