#### code to look at cpp output ####

library(data.table)

rm(data)
gc()

{data <- fread("migSimCode/testAgentPos.csv")
data <- data[,lapply(.SD, as.numeric), .SDcols = names(data)]
data <- na.omit(data)
head(data)}

# data[,nsd:=c(NA, diff(pos)),
#      by=list(id, gen)]

# plot data
library(ggplot2)
 x11()

randagent <- round(runif(200, 0, 99))

# histogram of peak distance at each timestep
setDT(data)
ggplot(data[gen %% 30 == 0,])+
  geom_histogram(aes(x = distpeak, group = gen),
                 fill = 2,
                 position = "identity", bins = 50, alpha = 0.7)+
  scale_fill_viridis_c()+
  geom_vline(xintercept = 0)+
  # scale_fill_
  facet_grid(gen~time, scales = "fixed", labeller = label_both)+
  theme_void()
  # xlim(-100, 100)+
  # labs(title = "wave velocity = 5.0, gens = 500")

ggsave("figTest01.png", dpi = 300, 
       device = png(), height = 8, width = 11, unit = "in"); dev.off()
