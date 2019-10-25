#### code to show initial assumptions ####

# Code author Pratik Gupte
# PhD student
# MARM group, GELIFES-RUG, NL
# Contact p.r.gupte@rug.nl

#### load libs ####

library(ggplot2)
# reverse log trans func
library("scales")
reverselog_trans <- function(base = exp(1)) {
  trans <- function(x) -log(x, base)
  inv <- function(x) base^(-x)
  trans_new(paste0("reverselog-", format(base)), trans, inv, 
            log_breaks(base = base), 
            domain = c(1e-100, Inf))
}

# themes
library(ggthemes)

#### prep resource ~ distance plot ####

div = 10^seq(-5,2,0.1)
a = 1/(1+(div))

# energy as a function of distance from peak
ggplot()+
  geom_path(aes(x = div, y = a), col = "dodgerblue", size = 1)+
  geom_vline(xintercept = c(0.1), col = 2, lty = 2)+
  scale_x_continuous(trans = reverselog_trans(10))+
  theme_fivethirtyeight()+
  theme(axis.title = element_text())+
  labs(x = "diff. from optimum", y = "resource gained")
