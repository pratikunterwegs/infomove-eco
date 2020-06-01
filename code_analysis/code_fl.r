library(data.table)

data = fread("data/info/fitness_landscape/1590214835366.csv")

library(dplyr)
count(data, replicate, mut_combo, a, b)

data[,n := .N,by = c("replicate", "mut_combo",
                     "a", "b")]

data = unique(data, by = c("replicate", "mut_combo",
                           "a", "b"))

data = data[,energy := energy - energy[n == max(n)],
            by = .(replicate, mut_combo)]

library(ggplot2)

pal = scico::scico(3, begin = 0.1, end = 0.9, palette = "vik")

ggplot(data)+
  geom_tile(aes(a,b,fill=energy),
            alpha = 1, col = "grey")+
  theme_test()+
  scale_fill_gradient2(low = pal[1],
                       high = pal[3],
                       mid = "white",
                       midpoint = 0)+
  coord_fixed()+
  facet_grid(mut_combo~replicate)
