#### network plotting ####

# load libs
library(data.table)
library(igraph)

# load and subset data
data = fread("data/dataOutStaticPeak.csv")
datasub = data[gen %% 5 == 0,]

# make ref df
refdf = expand_grid(id = 0:499, leader = 0:499)

# count interactions
library(dplyr)
library(tidyr)
library(purrr)
admat = datasub %>% 
  count(gen, id, leader) %>% 
  mutate(leader = ifelse(leader == -1, id, leader)) %>% 
  group_by(gen) %>%
  nest() %>%
  mutate(data = map(data, function(df){
    df = full_join(df, refdf)
    m = as.matrix(spread(df, leader, n, fill = 0, drop = FALSE))
    rownames(m) = m[,1]
    m = m[,-1]
    # check right dims
    assertthat::assert_that(nrow(m) == 500 && ncol(m) == 500,
                            msg = "matrix has wrong dimensions")
    return(m)
  }))

# make networks as list column
admat$network = map(admat$data, graph_from_adjacency_matrix)

# make df per gen of degree distributions
admat$networkdata = map(admat$network, function(network){
  tibble(prob = degree_distribution(network, mode = "in", loops = F),
         deg = 0:(length(prob)-1))
})

# prep single plot data
plotdata = admat %>% 
  select(gen, networkdata) %>% 
  unnest()

# themes
library(ggthemes)

# make plot
ggplot(plotdata)+
  geom_col(aes(x = deg, y = prob), fill = "dodgerblue")+
  facet_wrap(~gen)+
  xlim(0,50)+
  labs(x = "in degree (followers)", y = "degree distribution",
       title = "peak = 100, N agents = 500, genMax = 200, tMax = 10",
       subtitle = "init move = 0.0 : 100.0, init follow = -1.0 : 1.0")+
  theme_gdocs()

# save plot
ggsave(filename = "figs/figStaticPeakIndegDist.png", device = png(), width = 10, height = 8, dpi = 300); dev.off()

#### igraph network ####
b = admat$network[[2]]
b = igraph::simplify(b, remove.loops = TRUE, )
# choose a layout
layout <- layout.fruchterman.reingold(b)
# kleinberg hub and centrality
hub <- hub.score(b)$vector
# community detection
eb <- edge.betweenness.community(b)
membership <- cut_at(eb, no = 9)
# cluster edge betweeness
ceb = cluster_edge_betweenness(b)

# plot
{
  par(mar=c(0,0,0,0))
  plot(b, layout = layout,
       vertex.color = scales::alpha(pals::kovesi.rainbow(9), alpha = 0.8)[membership],
       vertex.size= scales::rescale(hub, c(1, 2)),
       edge.arrow.size = 0.2, vertex.label=NA,
       vertex.frame.color = NA,
       edge.color = scales::alpha("grey50", alpha = 0.2),
       asp = 0)
}
