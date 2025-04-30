$\displaystyle{\int}_0^x\dfrac{\sin{t}+1}{\sqrt{t}}dt=\alpha,\ \alpha\geq0$

Особенность в точке $t=0\Rightarrow$ замена: $t=u^2,\space{d}t=2udu,\quad{t}\in[0,x],u\in[0,\sqrt{x}]$

$\displaystyle{\int}_0^\sqrt{x}\dfrac{\sin{u^2}+1}{\sqrt{u^2}}2udu=2\int_0^\sqrt{x}(\sin{u^2}+1)du$

---

$\displaystyle{\int}_0^\infty\dfrac{\sin{t}+1}{\sqrt{t}}dt=\int_0^1\dfrac{\sin{t}+1}{\sqrt{t}}dt+\int_1^\infty\dfrac{\sin{t}+1}{\sqrt{t}}dt$

На отрезке $[0,1]\quad0\leq\dfrac{\sin{t}+1}{\sqrt{t}}\leq\dfrac{2}{\sqrt{t}}$

На промежутке $[1,+\infty)$

$\displaystyle{\int}_1^\infty\dfrac{\sin{t}+1}{\sqrt{t}}dt=\int_1^\infty\dfrac{\sin{t}}{\sqrt{t}}dt+\int_1^\infty\dfrac{1}{\sqrt{t}}dt$

$\displaystyle\int_1^\infty\dfrac{\sin{t}}{\sqrt{t}}dt$ сходится *по теореме Дирихле*

$\displaystyle\int_1^\infty\dfrac{1}{\sqrt{t}}dt$ расходится, значит весь интеграл расходится
