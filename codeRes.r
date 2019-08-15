#### code to look at cpp output ####

library(data.table)
library(ggplot2)
library(ggthemes)
x11()

rm(data);gc()

{data <- fread("migSimCode/peakdist.csv")
data <- data[,lapply(.SD, as.numeric), .SDcols = names(data)]
data <- na.omit(data)
head(data)}

# plot where agents are
d = fread("migSimCode/agentpos.csv")

# dist over time
ggplot(d[gen > 3e3,])+
  geom_histogram(aes(pos - peakpos), size = 0.1, col = 4, alpha = 0.1, bins = 150)+
#  geom_hline(yintercept = 0, col = 2)+
  # geom_line(aes(time, peakpos), col = 2, size = 0.1)+
  facet_grid(~gen)+
  xlim(-50, 100)

# energy over time
d[,intake:=c(NA, diff(energy)),by=list(gen,id)]
ggplot(d[gen > 3e3,])+
  geom_line(aes(time,intake, group = id), size = 0.1, alpha = 0.1)+
  # geom_line(aes(time, peakpos), col = 2, size = 0.1)+
  facet_grid(~gen)

# energy over distance
d[,dist:=c(NA, diff(pos)),by=list(gen,id)]
ggplot(d[gen > 3e3 & time == 99,])+
  geom_histogram(aes(neighbours+1), 
            size = 0.1, alpha = 0.8)+
  # geom_line(aes(time, peakpos), col = 2, size = 0.1)+
  facet_grid(~gen)+
  scale_x_log10()

# neighbours over time
ggplot(d[gen > 3e3,])+
  geom_path(aes(pos, neighbours, group = id), 
            alpha = 0.1, size = 0.1)+
  #coord_cartesian(ylim=c(0,1e2))+
  facet_grid(~gen, scales = "free")
