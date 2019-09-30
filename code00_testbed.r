#### code to test res dist ####

div = 10^seq(-5,2,0.1)
a = 1/(1+(div))

library(ggplot2)

ggplot()+
  geom_line(aes(x = div, y = a))+
  scale_x_log10()
