# Quantitative Trading System

## Basic Information

Quantitative trading is executed by defining mathematical models or algorithmic strategies that can be used to evaluate the profit of the strategy through historical data over time and events. It eliminates the emotional and cognitive biases that can affect people's trading decisions and also avoids potential costs and gain more profit by the fast and precise orders.


## Problem to Solve
Quantitative trading involves the following components, which we will carry out based on three time period strategies :
* Strategy selection
    * scalping trading : **A rapid, very short term trading style**
        * Depends on its ability to perform trading actions at critical points in time in an extremely latency-sensitive manner
        * HFT (High Frequency Trading)
    * swing trading : **A mid-term trading style**
        * Involves taking trades that last a couple of days up to several months in order to profit from an anticipated price move.
        * Technical analysis, analyzing price trend and patterns
    * position trading : **A long-term trading style**
        * Identify a trend and an investment that will benefit from it, then buy and hold the investment until the trend peaks
        * Trend following
* Strategy backtest
    * Backtesting for different time periods according to different strategies
    * Evaluate by Shape ratio, ROI and consider the potential costs, e.g. fees, slippage, etc
* Strategy execution
    * Need to quickly calculate the appropriate points of in and out
    * Ordering at precise points to avoid high slippage and other potential costs

Among them, **strategy execution** and **order** are particularly important for performance, since they directly affect the profit of the trading.

However we cannot have a hardware platform and network environment like a financial institution (the physical location of the hardware platform is important for High Frequency Trading). So we focus on the implementation of algorithm for **strategy execution**

## Prospective Users
Financial institutions trader, hedge funds, retail investors, people who want to make money from the secondary market

## System Architecture
![](https://i.imgur.com/BK5h5LN.png)

## API Description
### Python
* `crawler`
    * Crawl the market data for 10 years 
    * Store data in local database
* `parser(strategy, time_period, principal)`
    * Parse the user input of strategy
        * e.g. technical analysis indicator, mathematical models, etc
    * Construct an input table for later component
* `backtesting(strategy, time_period, principal)` 
    * Evaluate the profit and risk of the strategy
### C++
* `execution(risk, strategy)` 
    * According to the results of backtesting, this function can be used to evaluate the order position and  volume based on risk, profit and principal

* `order(position, volume)`
    * This function will place orders with the position and volume through the exchange (e.g. crypto exchange) API

## Engineering Infrastructure

* Build system : `make`
* Version control : `git`
* Testing framework : `pytest`

## Schedule
#### Week 1 (10/31)
* Design the strategy of each time period and finish `crawler`
#### Week 2 (11/7) 
* Implement the `backtesting` & `parser`
#### Week 3 (11/14) 
* Debug `backtesting` &`parser` and test these two features with `pytest`
#### Week 4 (11/21) 
* Implement the `execution` and finish the Python wrapper with `pybind` 
#### Week 5 (11/28) 
* Implement `order` & `execution` and integrate all features
#### Week 6 (12/5) 
* Test for all features
#### Week 7 (12/12) 
* Prepare the presentation
#### Week 8 (12/19) 
* Prepare the presentation


## Reference
* https://www.investopedia.com/terms/q/quantitative-trading.asp
* https://financetrain.com/how-to-build-your-own-quantitative-trading-strategy
* https://www.quantstart.com/articles/backtesting-systematic-trading-strategies-in-python-considerations-and-open-source-frameworks/
