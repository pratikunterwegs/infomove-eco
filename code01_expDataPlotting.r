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

#### diagnostic plot ###

ggplot()+
  geom_line(aes(x = 0:20, y = 25), 
            col = "darkgreen", size = 1, alpha = 0.1)+
  geom_line(data = land, aes(x = pos, y = food*2.5), 
            col = "red", size = 1, alpha = 0.2)+
  geom_jitter(data = data, aes(x = circPos, y = time, col = time), 
              alpha = 0.1)+
  
 
  
  coord_polar()+
  scale_colour_scico()+
  scale_x_continuous(breaks = c(0:19.9))+
  theme_bw()
