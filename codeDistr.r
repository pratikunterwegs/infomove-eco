#### code to check cpp issues ####

a = 1:1000
peak = 10
peakval = 1e2
steepness = 0.01
b = peakval^-(steepness*(abs(a-peak)))
plot(b~a, type="l")
