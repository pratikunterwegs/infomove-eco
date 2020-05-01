---
title: Model Results Version 05
subtitle: Parameters evolving with small mutations
documentclass: article
classoption: landscape
geometry: margin=0.2in
---

version/parameter|a|b|D|M|leader choices
---|---|---|---|---|---|---
05: Gaussian_a_b|Init: 0, evolving (-3 -- +3, Gaussian SD = 0.05)|Init: 0, evolving (-3 -- +3, Gaussian SD = 0.05)|Init: 0.5, evolving (0.0 -- 1.0)|Init: 2, evolving (0 -- 100)|Init: 1, 2, or 5; fixed globally


$p(follow) = 1/(1 + e^{-b(\Delta Q - a)})$

Movement cost per generation per individual = M (version 05)

Figure key: _a_, _b_ (following probability parameters); _D_ (aspiration level); _M_ (exploration distance); _pf_ (proportion of following)

# Version 05

## Type: No following

### Figures: Parameter trajectory (mean ± SD)

![](figs/fig_v5_noinfo_8.png)
![](figs/fig_v5_noinfo_9.png)
![](figs/fig_v5_noinfo_10.png)
![](figs/fig_v5_noinfo_11.png)

### Figures: Parameter counts

![](figs/fig_v5_noinfo_4.png)
![](figs/fig_v5_noinfo_5.png)
![](figs/fig_v5_noinfo_6.png)
![](figs/fig_v5_noinfo_7.png)

### Figures: Probability of following

![](figs/fig_v5_noinfo_0.png)
![](figs/fig_v5_noinfo_1.png)
![](figs/fig_v5_noinfo_2.png)
![](figs/fig_v5_noinfo_3.png)

## Type: Following allowed


### Figures: Parameter trajectory (mean ± SD)

![](figs/fig_v5_info_8.png)
![](figs/fig_v5_info_9.png)
![](figs/fig_v5_info_10.png)
![](figs/fig_v5_info_11.png)

### Figures: Parameter counts

![](figs/fig_v5_info_4.png)
![](figs/fig_v5_info_5.png)
![](figs/fig_v5_info_6.png)
![](figs/fig_v5_info_7.png)

### Figures: Probability of following

![](figs/fig_v5_info_0.png)
![](figs/fig_v5_info_1.png)
![](figs/fig_v5_info_2.png)
![](figs/fig_v5_info_3.png)