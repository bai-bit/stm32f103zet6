一条典型的ARM指令语法格式如下所示:
<opcode> {<cond>} {S} <Rn>,<Rd>,<shifter_operand>
opcode:指令操作符编码占4位
cond:指令执行的条件编码占4位
S:决定指令的操作是否影响CPSR的值占1位
Rn:目标寄存器占4位
Rd:第一个操作数寄存器占4位
shifter_operand:第二个操作数占12位