#### code to look at cpp output ####

library(data.table)

rm(data)
gc()

{data <- fread("migSimCode/testAgentPos.csv")
data <- data[,lapply(.SD, as.numeric), .SDcols = names(data)]
data <- na.omit(data)
head(data)}

data[,nsd:=c(NA, diff(pos)),
     by=list(agent, gen)]

# plot data
library(ggplot2)
# x11()

randagent <- round(runif(200, 0, 99))

ggplot(data[agent %in% randagent,])+
  geom_line(aes(x = time, y = pos, group = agent), size = 0.1)+
  facet_wrap(~gen)
