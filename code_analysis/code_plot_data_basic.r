# read data and plot figures

# load libs
library(dplyr)
library(tidyr)
library(readr)
library(purrr)

# plotting
library(ggplot2)
library(patchwork)

# custom func
prob_follow <- function(a, b, deltaq){
  return(1/(1 + exp(-b*(deltaq - a))))
}

# ci function
ci <- function(x) 1.96*sd(x, na.rm = T)/sqrt(length(x))

# read data files names and params
data_files <- list.files("data", pattern = "agent",
                         full.names = TRUE)
lookup <- read_csv("data/parameter_summary.csv")

# make df and read data as list column
data <- tibble(filename = data_files) %>%
  left_join(lookup, by = "filename")

data <- mutate(data, agent_data = map(filename, read_csv))

# unnest for data
data <- unnest(data, cols = "agent_data") %>%
  pivot_longer(cols = c("D", "a", "b", "F"))

data_summary <- data %>%
  filter(rep < 10) %>%
  mutate(value = round(value, digits = 1)) %>% 
  select(phi, gen, rep, name, value, init_d) %>%
  group_by(phi, gen, name, init_d, rep) %>%
  count(value)

# split by variable
data_summary <- ungroup(data_summary) %>% 
  split(data_summary, f = data_summary$name) %>% 
  map(function(df){ split(df, df$rep)})

# walk over the list
walk2(data_summary, names(data_summary), function(df_list, name){
  
  title = glue::glue('parameter: {name}')
  
  fig_subpanel <- map2(df_list, names(df_list), function(df, name2){
    
    subtitle = glue::glue('rep: {name2}')
    
    ggplot(df, aes(gen, value))+
      geom_tile(aes(fill = n))+  
      # geom_ribbon(alpha = 0.2, fill = fill)+
      # geom_line(size = 0.2, alpha = 0.9, col = fill)+
      scale_fill_viridis_c(option = "E", limits = c(0, 250),
                           na.value = "steelblue", direction = -1)+
      scale_x_continuous(breaks = c(0, 50, 100)*1000)+
      theme_minimal(base_size = 4)+
      theme(plot.background = element_rect(colour = "black", fill=NA,
                                           size = 0.2),
            panel.border = element_rect(colour = "black", fill=NA,
                                        size = 0.2),
            panel.grid = element_blank(),
            axis.title = element_text(),
            plot.title = element_text(face = "bold"),
            legend.position = "none")+
      # coord_cartesian(expand = F)+
      facet_grid(init_d~phi,labeller = label_both)+
      labs(subtitle = subtitle, x = NULL)
  })
  
  fig_param <- wrap_plots(fig_subpanel, guides = "collect", ncol = 3)+
    plot_annotation(title = title)
  
  ggsave(fig_param, filename = glue::glue('figs/fig_parameter_{name}.png'),
         width = 12, height = 8)
  
})

# show fig list
library(patchwork)
wrap_plots(fig_list, guides = "collect")

# get
