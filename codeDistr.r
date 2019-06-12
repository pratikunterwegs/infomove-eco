#### code to check cpp issues ####

a = 1:1000
peak = 10
peakval = 1e2
steepness = 0.01
b = peakval^-(steepness*(abs(a-peak)))
plot(b~a, type="l")

#### check green wave ####

library(tidyverse)

land <- read_csv("migSimCode/testLandOutput.csv", col_names = T)
land <- mutate(land, time = 1:nrow(land)) %>% 
  gather(site, resources, -time)

ggplot(land)+
  geom_col(aes(x = as.numeric(site), y = resources, fill = resources),
           position = "identity", alpha = 0.5)+
  #scale_y_reverse()+
  scale_fill_viridis_c(limits = c(0,1), option = "plasma")#+
  theme_void()
