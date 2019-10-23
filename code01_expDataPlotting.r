#### code to analyse data ####

rm(list = ls())

# load libs
library(data.table)
library(ggplot2)
library(ggthemes)

# ci func
ci = function(x){qnorm(0.975)*sd(x, na.rm = T)/sqrt(length(x))}

# load datas
data = fread("migSimCode/dataOutExp.csv")

# make numeric
data = data[,lapply(.SD, as.numeric), .SDcols = c(names(data))]

dataSub = data[gen %% 5 == 0,]

#### make plots ####
# plot difference from optimum of used and inherited param
{
  dataSum = dataSub[,.(gen,id,movep,movepcopy)]
  dataSum = melt(dataSum, id.vars = c("gen","id"))
  dataSum = dataSum[,`:=`(roundval = plyr::round_any(value, 100))
                  ][,.N, by = c("gen", "roundval","variable")]
}

# labels
labels = c(movep = "inherited", movepcopy = "used")

# plot fig
ggplot(dataSum)+
  geom_tile(aes(x = gen, y = roundval, fill = N))+
  facet_wrap(~variable, labeller = labeller(variable = labels))+
  scale_fill_viridis_c(option = "B", direction = -1)+
  theme_gdocs()+
  ylim(0, 2000)+
  labs(x = "generation", y = "movement distance",
       title = "depletable land, N agents = 500, genMax = 200, tMax = 10",
       subtitle = "init move = 0.0 : 100.0, init follow = -1.0 : 1.0")

# export plot
ggsave(filename = "figs/figExploit.png", device = png(), width = 10, 
       height = 8, dpi = 300); dev.off()