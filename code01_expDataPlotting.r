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
data = fread("migSimCode/dataOutExp05.csv")

# make numeric
data = data[,lapply(.SD, as.numeric), .SDcols = c(names(data))]

dataSub = data#[gen %% 5 == 0,]

#### make plots ####
# plot difference from optimum of used and inherited param
{
  dataSum = dataSub[time %% 5 == 0,.(gen,id,movep,movepcopy,time)
                    ]
  dataSum = melt(dataSum, id.vars = c("gen","id","time"))
  dataSum = dataSum[,`:=`(roundval = plyr::round_any(value, 5))
                  ][,.N, by = c("gen","time", "roundval","variable")]
}

# labels
labels = c(movep = "inherited", movepcopy = "used", diff = "difference")

# plot fig
ggplot(dataSum)+
  geom_tile(aes(x = (gen), y = roundval, fill = N))+
  facet_grid(time~variable, labeller = labeller(variable = labels))+
  scale_fill_scico()+
  # scale_x_continuous(breaks = seq(0, 1e3, 25))+
  theme_void()+
  ylim(-10, 100)+
  labs(x = "generation", y = "movement distance",
       title = "depletable land, N agents = 500, genMax = 1000, tMax = 25",
       subtitle = "init move = 0.0, init follow = -10.0 : 10.0")


# export plot
ggsave(filename = "figs/figGenExploit.png", device = png(), width = 10, 
       height = 8, dpi = 300); dev.off()

#### plot of leadership behaviour ####
{
  dataLead = dataSub[,.(gen,id,movep,movepcopy)
                     ][,bfollow:=(movepcopy-movep)!=0]
  dataLead = dataLead[,.(prop = .N), by = c("gen", "bfollow")]
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
  dataChain = data[,.(prop = .N), by = c("gen", "chainlength")]
}

ggplot(dataChain)+
  geom_tile(aes(x = gen, y = chainlength, fill = prop))+
  scico::scale_fill_scico(direction = 1)+
  ggthemes::theme_clean()
