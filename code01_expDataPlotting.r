#### code to analyse data ####

rm(list = ls())

# load libs
library(data.table)
library(ggplot2)
library(ggthemes)
library(scico)

# ci func
ci = function(x){qnorm(0.975)*sd(x, na.rm = T)/sqrt(length(x))}

# load datas
data = fread("migSimCode/dataOutExp06.csv")

# make numeric
data = data[,lapply(.SD, as.numeric), .SDcols = c(names(data))]

dataSub = data#[gen %% 5 == 0,]

#### make plots ####
# plot difference from optimum of used and inherited param
{
  dataSum = dataSub[gen %% 5 == 0,.(gen,id,movep,movepcopy)              ]
  dataSum = melt(dataSum, id.vars = c("gen","id"))
  #dataSum = dataSum[,`:=`(roundval = plyr::round_any(value, 5))
   #               ][,.N, by = c("gen", "roundval","variable")]
}

# labels
labels = c(movep = "inherited", movepcopy = "used", diff = "difference")

x11()
# plot fig
ggplot(dataSum)+
  geom_density(aes(x = value, fill = variable), alpha=0.6,
               col = "transparent")+
  facet_grid(~gen, 
             labeller = labeller(variable = labels),
             scales = "free_y")+
  scale_fill_scico_d(palette = "roma")+
  # scale_x_continuous(breaks = seq(0, 1e3, 25))+
  theme_clean()+
  xlim(-10, 40)+
  coord_cartesian(ylim=c(0, 0.2))+
  labs(x = "generation", y = "movement distance",
       title = "depletable land, N agents = 100, genMax = 1000, tMax = 25",
       subtitle = "init move = 0.0, init follow = 0.0")


# export plot
ggsave(filename = "figs/figGenExploit.png", device = png(), width = 10, 
       height = 8, dpi = 300); dev.off()

#### plot of leadership behaviour ####
{
  dataLead = dataSub[,.(gen,id,movep,movepcopy)
                     ][,bfollow:=(movepcopy-movep)!=0]
  dataLead = dataLead[,.(prop = .N/2500), by = c("gen", "bfollow")]
}

# plot fig
ggplot(dataLead)+
  geom_tile(aes(x = gen, y = bfollow, fill = prop))+
  scico::scale_fill_scico(direction = 1)+
  scale_y_discrete(labels = c("indep.", "follow"))+
  ggthemes::theme_clean()+
  labs(x = "generation", y = "strategy",
       title = "depletable land, N agents = 500, genMax = 200, tMax = 10",
       subtitle = "init move = 0.0 : 100.0, init follow = -1.0 : 1.0")

# export plot
ggsave(filename = "figs/figFollowPropExpCopy.png", device = png(), width = 10, height = 4, dpi = 300); dev.off()

#### chain length over time ####
# prep data
{
  dataChain = data[,.(prop = .N), by = c("gen", "time", "chainlength")]
}

ggplot(dataChain)+
  geom_tile(aes(x = gen, y = chainlength, fill = prop))+
  scico::scale_fill_scico(direction = 1)+
  facet_wrap(~time)+
  ggthemes::theme_clean()
