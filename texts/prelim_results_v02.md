---
title: Model Results Version 02
subtitle: some evolving parameters
documentclass: article
classoption: landscape
geometry: margin=0.2in
---

version/parameter|D|M|leader choices
---|---|---|---|---
02: free_a_b_fixed_d|Init: 1.0, fixed|Init: 2, fixed|Init: 1, 2, or 5; fixed globally
03: free_a_b_d|Init: 0.5, evolving (0.0 -- 1.0)|Init: 2, fixed|Init: 1, 2, or 5; fixed globally
04: free_a_b_d_m|Init: 0.5, evolving (0.0 -- 1.0)|Init: 2, evolving (0 -- 100)|Init: 1, 2, or 5; fixed globally


$p(follow) = 1/(1 + e^{-b(\Delta Q - a)})$

Movement cost per generation per individual = M (version 04)

Figure key: _a_, _b_ (following probability parameters); _D_ (aspiration level); _M_ (exploration distance); _pf_ (proportion of following)

# Version 02

## Type: No following

### Figures: Parameter trajectory (mean ± SD)

![](figs/fig_v2_20.png)
![](figs/fig_v2_21.png)
![](figs/fig_v2_22.png)
![](figs/fig_v2_23.png)

### Figures: Parameter counts

![](figs/fig_v2_16.png)
![](figs/fig_v2_17.png)
![](figs/fig_v2_18.png)
![](figs/fig_v2_19.png)

## Type: Following allowed

### Figures: Parameter trajectory (mean ± SD)

![](figs/fig_v2_8.png)
![](figs/fig_v2_9.png)
![](figs/fig_v2_10.png)
![](figs/fig_v2_11.png)

### Figures: Parameter counts

![](figs/fig_v2_4.png)
![](figs/fig_v2_5.png)
![](figs/fig_v2_6.png)
![](figs/fig_v2_7.png)

# Version 03

## Type: No following

### Figures: Parameter trajectory (mean ± SD)

![](figs/fig_v3_16.png)
![](figs/fig_v3_17.png)
![](figs/fig_v3_18.png)
![](figs/fig_v3_19.png)

## Type: Following allowed

### Figures: Parameter trajectory (mean ± SD)

![](figs/fig_v3_8.png)
![](figs/fig_v3_9.png)
![](figs/fig_v3_10.png)
![](figs/fig_v3_11.png)

### Figures: Parameter counts

![](figs/fig_v3_4.png)
![](figs/fig_v3_5.png)
![](figs/fig_v3_6.png)
![](figs/fig_v3_7.png)

# Version 04

## Type: No following

### Figures: Parameter trajectory (mean ± SD)

![](figs/fig_v5_20.png)
![](figs/fig_v5_21.png)
![](figs/fig_v5_22.png)
![](figs/fig_v5_23.png)

### Figures: Parameter counts

![](figs/fig_v5_16.png)
![](figs/fig_v5_17.png)
![](figs/fig_v5_18.png)
![](figs/fig_v5_19.png)

## Type: Following allowed

### Figures: Parameter trajectory (mean ± SD)

![](figs/fig_v5_8.png)
![](figs/fig_v5_9.png)
![](figs/fig_v5_10.png)
![](figs/fig_v5_11.png)

### Figures: Parameter counts

![](figs/fig_v5_4.png)
![](figs/fig_v5_5.png)
![](figs/fig_v5_6.png)
![](figs/fig_v5_7.png)
