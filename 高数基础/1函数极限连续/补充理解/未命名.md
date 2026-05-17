---
author: zwt
date: 2026-05-17
tags:
  - 考研
  - 高数/基础
描述:
---
### 1. 当 $f(a) = 0$ 时（零点处的尖点）

这是最常见的情况，例如 $f(x) = |x|$ 在 $x=0$ 处。

根据导数定义，由于 $f(a) = 0$，必然有 $z(a) = 0$。我们需要计算左右导数：

$$ z'_\pm(a) = \lim_{x \to a^\pm} \frac{f(x)g(x) - 0}{x - a} = \lim_{x \to a^\pm} \left[ g(x) \cdot \frac{f(x) - f(a)}{x - a} \right] $$

因为 $\lim_{x \to a^\pm} \frac{f(x) - f(a)}{x - a} = f'_\pm(a)$，所以：

- 右导数：$z'_+(a) = \lim_{x \to a^+} g(x) \cdot f'_+(a)$
    
- 左导数：$z'_-(a) = \lim_{x \to a^-} g(x) \cdot f'_-(a)$
    

**充要条件：**

上述两个单侧极限必须存在且相等。即：

$$ \lim_{x \to a^+} g(x) \cdot f'_+(a) = \lim_{x \to a^-} g(x) \cdot f'_-(a) $$

> **核心推论：**
> 
> 如果已知 $g(x)$ 在 $a$ 点是**连续**的，那么 $\lim_{x \to a^+} g(x) = \lim_{x \to a^-} g(x) = g(a)$。
> 
> 代入充要条件得：$g(a)f'_+(a) = g(a)f'_-(a)$。由于尖点要求 $f'_+(a) \neq f'_-(a)$，这**强制要求 $g(a) = 0$ **。
> 
> _(即：如果 $g$ 连续，它必须提供一个零点来“压平” $f$ 的尖点。)_