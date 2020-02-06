#### code to plot smootherstep ####

smootherstep <- function(x, deadzone, deprange) {
  # // Scale, and clamp x to 0..1 range
  if (x < deadzone){
    x = deadzone}
  if (x > deprange){
    x = deprange}
  x = (x-deprange)/(deadzone-deprange)
  # // Evaluate polynomial
  return (x * x * x * (x * (x * 6 - 15) + 10));
}