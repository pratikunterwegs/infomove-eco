# read data and plot figures

# load libs
library(dplyr)
library(tidyr)
library(ggplot2)

# read data files
data_files <- list.files("data/", pattern = "agent",
                         full.names = TRUE)
data <- purrr::map(data_files, read_csv)
lookup <- read_csv("data/parameter_summary.csv")
