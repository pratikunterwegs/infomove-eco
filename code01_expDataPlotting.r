#### code to analyse data ####

# load libs
library(data.table)
library(ggplot2)
library(ggthemes)

# ci func
ci = function(x){qnorm(0.975)*sd(x, na.rm = T)/sqrt(length(x))}

# load datas
data = fread("migSimCode/dataOut.csv")

dataSub = data[gen %% 5 == 0,]

#### make plots ####
# plot difference from optimum of used and inherited param
{
  dataSum = dataSub[,.(gen,id,movep,movepcopy)]
  dataSum = melt(dataSum, id.vars = c("gen","id"))
  dataSum = dataSum[,`:=`(peakdiff = abs(100 - value),
                        roundval = plyr::round_any(value, 5))
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
  coord_cartesian(ylim=c(-10,150))+
  labs(x = "generation", y = "movement distance",
       title = "peak = 50/100, N agents = 500, genMax = 200, tMax = 10",
       subtitle = "init move = 0.0 : 100.0, init follow = -1.0 : 1.0")

# export plot
ggsave(filename = "figs/figAltPeak.png", device = png(), width = 10, 
       height = 8, dpi = 300); dev.off()