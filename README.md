# Metrics
Tracks how many times each metamod hook is called. I used this to troubleshoot performance issues on Linux, where metamod hooks are really expensive for some reason. Disabling the most frequently called hooks solves the performance problem, but probably breaks a lot of metamod plugins.

Commands:  
`metrics` show hook call counts for the current map  
`metrics_reset` clear call counts
