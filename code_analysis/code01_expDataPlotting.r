#### code to analyse data ####

rm(list = ls())

# load libs
library(data.table)
library(ggplot2)
library(ggthemes)
library(scico)

# ci func
ci = function(x){qnorm(0.975)*sd(x, na.rm = T)/sqrt(length(x))}

# load agents
data = fread("migSimCode/dataAgents.csv")
# load land
land = fread("migSimCode/dataLand.csv")

# make numeric
data = data[,lapply(.SD, as.numeric), .SDcols = c(names(data))]
land = land[,lapply(.SD, as.numeric), .SDcols = c(names(land))]

#### diagnostic plot ###

ggplot()+
  # geom_line(aes(x = 0:20, y = 25),
  #           col = "darkgreen", size = 1, alpha = 0.1)+
  geom_path(data = land, aes(x = pos, y = food, group = t),
            col = 4, size = 1, alpha = 0.2)+
  # geom_point(data = data, aes(x = circPos, y = time), 
  #             alpha = 0.1, col = 2)+
  #coord_polar(start = 0)+
  facet_wrap(~gen)+
  scale_colour_scico(palette = "lajolla")+
  scale_x_continuous(breaks = c(0:20))+
  theme_bw()
