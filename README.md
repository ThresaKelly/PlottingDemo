### Thresa Kelly | January 2024

---

This repository contains the Qt Creator project, **PlottingDemo**. This project contains several examples to display the functionality of [QCustomPlot](https://www.qcustomplot.com/). There are four different tabs at the top of MainWindow: *Assorted*, *Time Series*, *Heat Map*, and *Doodle*. 

*Assorted* contains several tutorial plots provided in the QCustomPlot documentation--with the exception of Range Select and Poke that I made myself. Range Select allows the user to click and drag on the plot to highlight an area. Poke allows the user to click on the graph to add points to the data series. 

*Time Series* generates a time series graph of two noisy sinusoids. The cosine curve's start time is delayed by the amount selected in the options menu. The user can also select their preferred time axis. Clicking the "start" button begins generating and plotting data. While running, random annotations (red boxes with text) will be generated after the cosine delay time is complete. There are also two horizontal lines marking the amplitude that the user can click and drag to move. Clicking the "stop" button will end data generation, and the user can then drag the graph around. 

*Heat Map* plots a two-dimensional histogram of a dipole's magnetic field strength. The user can select the colorbar from the options provided by QCustomPlot. The user can also increace or decreace the pixel resolution, or bin size, of the heat map using the range selector bar. Checking the "Interpolate" checkbox will interpolate gradients between each bin, rather than having harsh color lines. Clicking the "start" button will begin expanding and contracting the plot data (the axis remain fixed). 

*Doodle* allows the user to draw shapes directly onto the graph. The user clicks and drags on the graph to draw a line. Checking the "Fill next shape" checkbox in the options menu will fill the next drawn line with a transparent color. Checking the "Snap end to start" box will connect the end of the drawn line to its starting point. The user can also select the pen size and color to draw with. The user can also randomize the line/fill color for the next shape by checking the "Use random color" box. Clicking the "Reset" button will remove all the shapes on the graph.