
module load R/3.6.1-foss-2018a

Rscript --slave -e 'devtools::install_github("pratikunterwegs/infomover", upgrade = "never")'
