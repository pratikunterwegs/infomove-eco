#### code to show initial assumptions ####

# Code author Pratik Gupte
# PhD student
# MARM group, GELIFES-RUG, NL
# Contact p.r.gupte@rug.nl

# source smootherstep
source("smootherstep.r")
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
{
  png(filename = "fig_smootherstep.png", res = 300, height = 500, width = 800)
  x = seq(0,100,0.01)
  maxland = 100
  pos = 95
  # this is the distance on a ring
  diffpos = matrix(c(abs(x - pos), maxland - abs(x - pos)), ncol = 2)
  distonring = apply(diffpos, 1, min)
  dep = sapply(distonring, smootherstep, deadzone=5, deprange=20)
  dep2 = sapply(distonring, smootherstep, deadzone=10, deprange=40)
  #dep3 = sapply(distonring, smootherstep, deadzone=5, deprange=20)
  res = 10 - dep
  res2 = 10 - dep2
  #res3 = 10-dep3
  library(ggplot2)
  
  print(ggplot()+
          geom_line(aes(x, res))+
          geom_path(aes(x, res2), colour="red")+
          geom_vline(xintercept = c(95-10,95-40), lty = 2, colour = "red")+
          geom_vline(xintercept = 95, colour="blue")+
          ggthemes::theme_few()+
          labs(x = "position", y = "resource value"))
  dev.off()
}

#### angle issues ####
{
  angle = seq(-2*pi, 2*pi, 0.001)
  prop = (sin(angle) + 1)/2
  plot(prop~angle, type = "l")
  abline(h = 0.5, col =2)
}