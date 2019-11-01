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
data = fread("migSimCode/dataOut.csv")

# make numeric
data = data[,lapply(.SD, as.numeric), .SDcols = c(names(data))]

dataSub = data#[gen %% 5 == 0,]

#### make plots ####
# plot difference from optimum of used and inherited param
{
  dataSum = dataSub[#(gen %% 5 == 0) & 
    (time %% 4 == 0),.(gen,id,movep,movepcopy,time)]
  dataSum = melt(dataSum, id.vars = c("gen","id","time"))
  dataSum = dataSum[,`:=`(roundval = plyr::round_any(value, 1))
                    ][,.N, by = c("gen", "roundval","variable", "time")]
}

# labels
labels = c(movep = "inherited", movepcopy = "used", diff = "difference")

x11()
# plot fig
ggplot(dataSum)+
  geom_tile(aes(x = gen, y = roundval, fill = N))+
  facet_grid(time~variable, 
             labeller = labeller(variable = labels),
             scales = "free_y")+
  scale_fill_scico(limits = c(0, 60),
                   palette = "lajolla",
                   na.value = "blue")+
  # scale_x_continuous(breaks = seq(0, 1e3, 25))+
  theme_clean()+
  ylim(0, quantile(dataSum$roundval, 0.95))+
  # coord_cartesian(ylim=c(0, 10))+
  labs(x = "generation", y = "movement distance",
       title = "depletable land, N agents = 100, genMax = 1000, tMax = 25",
       subtitle = "init move = 0.0, init follow = 0.0")

# export plot
ggsave(filename = "figs/figGenExploit.png", device = png(), width = 8, 
       height = 12, dpi = 300); dev.off()

#### plot of leadership behaviour ####
{
  dataLead = dataSub[#(gen %% 5 == 0) & 
    (time %% 5 == 0),
    .(gen,id,movep,movepcopy,time)
    ][,bfollow:=(movepcopy-movep)!=0]
  dataLead = dataLead[,.(prop = .N), by = c("gen", "time", "bfollow")]
}

# plot fig
ggplot(dataLead)+
  geom_tile(aes(x = gen, y = bfollow, fill = prop))+
  scale_fill_distiller(palette = "RdBu")+
  scale_y_discrete(labels = c("indep.", "follow"))+
  facet_grid(rows = vars(time))+
  ggthemes::theme_clean()+
  labs(x = "generation", y = "strategy",
       title = "depletable land, N agents = 100, genMax = 1000, tMax = 25",
       subtitle = "init move = 0.0 : 0.0, init follow = 0.f")

# export plot
ggsave(filename = "figs/figFollowPropExpCopy.png", device = png(), 
       width = 8, height = 12, dpi = 300); dev.off()
# 
# #### chain length over time ####
# # prep data
# {
#   dataChain = data[(gen %% 5 == 0) & (time %% 2 == 0),.(prop = .N), by = c("gen", "time", "chainlength")]
# }
# 
# ggplot(dataChain)+
#   geom_tile(aes(x = gen, y = (chainlength), fill = prop))+
#   scico::scale_fill_scico(direction = -1, palette = "roma")+
#   facet_grid(rows = vars(time))+
#   coord_cartesian(ylim = c(0, 5))+
#   ggthemes::theme_few()
# 
# # export plot
# ggsave(filename = "figs/figFollow.png", device = png(), 
#        width = 8, height = 12, dpi = 300); dev.off()

#### read and plot land
{
  land = fread("migSimCode/landOut.csv")[gen %% 5 == 0,]
  # land = melt(land, id.vars = c("gen", "pos"))
  # land = land[,propval:=value/max(value),
  #             by = c("gen","variable")]
}

ggplot(land)+
  geom_tile(aes(x = pos, y = gen, fill = visits))+
  # facet_wrap(~variable)+
  scale_fill_distiller(palette = "YlOrRd", 
                       limits = c(0, 1000),
                       na.value = "white", 
                       direction = 1)+
  scale_y_reverse()+
  ggthemes::theme_clean()
