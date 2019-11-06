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

#### looking at continuous agent depletion ####
# is the distribution wrapped?
x = seq(0,100,0.01)
maxland = 100
pos = 95
# this is the distance on a ring
diffpos = matrix(c(abs(x - pos), maxland - abs(x - pos)), ncol = 2, byrow = T)
distonring = apply(diffpos, 1, min)
res = 1/(1 + exp(-5*(distonring - 3)))
#res = 1 - dep
plot(x, res, type = "l")
abline(h = 0, col = 2)
