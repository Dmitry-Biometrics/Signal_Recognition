1. PURPOSE OF THE PROGRAM
1.1. The software provides the following functions:
- adding examples of signals from txt, xlsx, bin files and directories;
- adding examples using the unsupported method;
- adding examples with synchronization on the clock generator bus or data;
- generation of artificial signal sequences;
- selection and subsampling of examples for various conditions;
- evaluation of the characteristics of examples of signals (signal-to-noise ratio, energy, statistical moments, quality);
- processing of examples (scaling, normalization, noise pollution, application of mathematical functions, marking)
- setting up various types of classifiers, including those made in accordance with GOST R 52633.4, fully connected neural networks, convolutional neural networks;
- testing the quality of training of the classifier and its subsequent use to classify examples of signals, as well as to search for similar signals in a continuous data stream.
2. CONDITIONS FOR PERFORMANCE OF THE PROGRAM
2.1. The software is a set of modules, including the SignalExp executable file, which is installed on the PC.
2.2. The software runs under Linux.
2.3. For the full functioning of the software, you need to install the Python development environment and the Qt, NumPy, Matplotlib, Keras, TensorFlow libraries.
2.4. No software maintenance required.

3. IMPLEMENTATION OF THE PROGRAM
H.1. Program installation
The software is installed by copying the files of the SignalExp catalog to a PC and installing additional libraries mentioned above.
3.2. Calling and loading the program
The program is launched by launching the installed SignalExp application from the program directory.
3.3. Main window and menu of the program

The main window is shown in Figure 1.
![image](https://user-images.githubusercontent.com/16018075/135731628-01a9e547-62bf-4dda-886d-74ab2521ccac.png)


 
The window includes the following areas:
- menu (1);
- classifiers (2);
- groups (3);
- examples (4);
- button panels (5);
- graphs (6);
- log (7).

The menu area is for activating specialized software functions. The main menu of the program includes the following submenus: "File", "Groups", "Examples", "Processing", "Classifiers".
    
![image](https://user-images.githubusercontent.com/16018075/135731641-84a20ca7-314e-415a-94a6-da86171b7680.png)

     
The File submenu allows you to add data from various sources, save examples to one or more files, and exit the program.
The "Groups" submenu is designed to manage groups, namely, to create a new and delete a previously created group.
In the "Examples" submenu, some operations for working with examples are taken out, namely, inverting the selected examples in the list of the active group, marking the examples of the group, creating artificial examples with a given noise level.
The "Processing" submenu contains functions responsible for processing the selected sets of examples.
The "Classifiers" submenu allows you to train neural network and other classifiers for solving problems of signal identification and automatic marking of the signal flow.
The "Classifiers" area contains a list of classifiers trained since the start of the application, which can be used to test signals.
The "Groups" area displays the loaded sets of examples in a tree-like form (Fig. 3). The operations of adding a new group ("G +" button) and deleting the selected group ("G-" button) are supported. In addition, this panel contains a button for marking examples (that is, assigning them to one of the classes for training or according to the results of training).
 
 
![image](https://user-images.githubusercontent.com/16018075/135731655-f3221a7d-eb1c-478e-a7a6-64a5d44974d6.png)

Fig. 3
The Examples area contains a list of examples (signals) included in the group selected in the Data and Results tree. The area is shown in Fig. 4.
 
 
![image](https://user-images.githubusercontent.com/16018075/135731659-2bc81d88-4d1b-40ef-b070-645e4b13cf21.png)

Fig. 4.

The main buttons for managing the lists of examples are also located here:
- adding examples from the selected files ("+" button);
- adding examples from all files in the catalog ("+ D" button);
- adding examples on a signal of unsupported methods (button "+ Rcr");
- adding examples 0 and 1 on the synchronization bus (button "+ Bus");
- adding synthetic examples using random noise generators (+ Gen button);
- output the value of the example to the log (button "Зн");
- selection of examples from a group (or from a previously selected set) by condition ("select" button);
- copying of selected examples or all examples of a group into a new group ("copy to" button);
- inversion of selection of examples (button "/");
- deleting the selected examples (button "-");
- saving selected examples to a set of text files or one binary file (">>" button).

The "Button Panel" area contains buttons that implement the main functionality of the program (Fig. 5):
- training the neural network using external means (button "Train external");
- training of the neural network, made in accordance with GOST R 52633.4 and other types of classifiers (the "Train" button);
- setting the parameters of the classifier after the completion of training (button "...");
 
 
![image](https://user-images.githubusercontent.com/16018075/135731666-3a6f5013-e1de-4535-9a97-f06f600b26cb.png)

Fig. 5
- testing the trained classifier on the selected examples (button "Test");
- control over the visibility of the chart area (the "Chart" button);
- managing the visibility of the log area (the "Log" button);
- removal of text information from the log area (button "OchLog").

The area of graphs is intended for displaying the data of an example (signal) in the form of a graph (Fig. 6.). It directly contains the area for drawing graphs of signals - along the X axis, the signal counts are plotted (one value - one count), the Y-axis is the count value. By default, the graph is drawn for the example that is highlighted in the examples area. If more than one example is selected, only the first one is displayed. The buttons panel above the drawing area is used to control various display options.
 
 
![image](https://user-images.githubusercontent.com/16018075/135731672-23806448-e85c-4e7d-ac9b-9c04ea072647.png)

Fig. 6
The following actions can be performed using the drawing area button bar:
- disabling the function of autoscaling the axes of the graphs according to the example data (checkbox "fixed axes");
- selection of the display color of the current example ("color" button);
- adding the current chart for comparison with the new chart ("+ fix" button);
- deleting all previously added charts ("clear" button);
- moving the current chart relative to the charts added earlier (buttons "up", "down").

The area for displaying text information is intended to display a report on the process of a certain function of the program and the results of working out the function (Fig. 7.). In addition, at the bottom of the area there are text fields responsible for displaying information about the first selected example in the example area:
- the "Class" field contains information about the example markers: the original example class (0, 1, ..) and the class obtained from the test results using some classifier;
- the "Length" field informs about the number of samples in the example;
- the "Response" field contains the value returned by the example classifier.
 
 
 ![image](https://user-images.githubusercontent.com/16018075/135731679-05e70e36-73f5-42db-b194-38caac344f51.png)
 
Fig. 7
