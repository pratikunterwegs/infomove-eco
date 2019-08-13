#### code to look at cpp output ####

library(data.table)
library(ggplot2)
library(ggthemes)
x11()

rm(data);gc()

{data <- fread("migSimCode/testAgentPos.csv")
data <- data[,lapply(.SD, as.numeric), .SDcols = names(data)]
data <- na.omit(data)
head(data)}

# histogram of peak distance at each timestep
#setDT(data)
ggplot(data[time %% 30 == 0,])+
  geom_histogram(aes(x = distpeak, group = gen),
                 fill = 2,
                 position = "identity", bins = 50, alpha = 0.7)+
  scale_fill_viridis_c()+
  geom_vline(xintercept = 0, col=4)+
  facet_grid(gen~time, scales = "fixed", labeller = label_both)+
  theme_few()
  # labs(title = "wave velocity = 5.0, gens = 500")

ggsave("figTest01.png", dpi = 300, 
       device = png(), height = 8, width = 11, unit = "in"); dev.off()
