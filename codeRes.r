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

# histogram of peak distance at each timestep
#setDT(data)
ggplot(data[time %% 20 == 0 & gen %% 50 ==0,])+
  geom_histogram(aes(x = distpeak, group = gen),
                 fill = 2, binwidth = 10,
                 position = "identity", bins = 50, alpha = 0.7)+
  scale_fill_viridis_c()+
  geom_vline(xintercept = 1, col=4)+
  facet_grid(gen~time, scales = "fixed", labeller = label_both)+
  coord_cartesian(xlim = c(-1e2, 1e2))
  # labs(title = "wave velocity = 5.0, gens = 500")


# plot where agents are
d = fread("migSimCode/agentpos.csv")

# dist over time
ggplot(d[id %% 10 == 0,])+
  geom_point(aes(time, pos), size = 0.1, alpha = 0.1)+
  geom_line(aes(time, peakpos), col = 2, size = 0.1)+
  facet_grid(~gen)

# energy over time
ggplot(d)+
  geom_point(aes(time, energy), size = 0.1, alpha = 0.1)+
  # geom_line(aes(time, peakpos), col = 2, size = 0.1)+
  facet_grid(~gen)

# energy over distance
d[,dist:=c(NA, diff(pos)),by=list(gen,id)]
ggplot(d[gen > 150,])+
  geom_point(aes(time, dist), size = 0.1, alpha = 0.1)+
  # geom_line(aes(time, peakpos), col = 2, size = 0.1)+
  facet_grid(~gen)
