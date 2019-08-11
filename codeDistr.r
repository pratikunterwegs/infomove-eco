#### code to check cpp issues ####

a = 1:1000
peak = 10
peakval = 1e2
steepness = 1e-3
b = peakval^-(steep*(abs(a-peak)))
plot(b~a, type="l")

# function for peak
wave = function(peak, peakval, steep, site){
  peakval^(-steep*(abs(site-peak)))
}

library(tidyverse)

z = crossing(peakloc = seq(10, 1000, 100),
           peakval = seq(1, 1e2, 1e1),
           steepness = 10^(-4:-1),
           site = 1:500)

z = mutate(z, val = wave(peakloc,peakval,steepness,site))

# plot data
ggplot(z)+
  geom_tile(aes(site, peakloc, fill = val))+
  facet_grid(peakval~steepness, labeller = label_both)+
  scale_fill_distiller(palette = "BrBG", direction = 1)
