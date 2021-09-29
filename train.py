#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy
import struct
import matplotlib.pyplot as plt
from keras.models import Sequential
from keras.layers import Dense
from scipy.stats._continuous_distns import foldcauchy_gen

def readSignal(path):
    file = open(path, "rb")
    bytes = file.read()
    data = numpy.frombuffer(bytes, dtype=numpy.float32)
    file.close()
    return data

def readbin(path):
    file = open(path, "rb")
    rows = struct.unpack('<i', file.read(4))[0]
    cols = struct.unpack('<i', file.read(4))[0]
    print("Read matrix rows=" + str(rows) + " cols=" + str(cols))
    bytes = file.read()
    data = numpy.frombuffer(bytes, dtype=numpy.float32)
    data = data.reshape(rows, cols)
    return data

def getFrames(data, win_size, step, shifts):
    frames = []
    for i in shifts:
        shift_data = data[i:len(data)]
        len_shift = len(shift_data)
        for j in range(0, len_shift, step):
            if len_shift > j + win_size:
                frames.extend([shift_data[j:(j + win_size)]])
    return numpy.array(frames)

def prepDataSet(dataSet0, dataSet1, split0=0.5, split1=0.5, cross=True):
    len0 = len(dataSet0)
    len1 = len(dataSet1)
    trainLen0 = int(len0 * split0)
    trainLen1 = int(len1 * split1)
    minTrainLen = numpy.min([trainLen0, trainLen1])
    if cross is True:
        xtrain = []
        ytrain = []
        for i in range(int(minTrainLen)):
            xtrain.extend([dataSet0[i]])
            xtrain.extend([dataSet1[i]])
            ytrain.extend([True])
            ytrain.extend([False])
        x_train = numpy.array(xtrain)
        y_train = numpy.array(ytrain)
        x_test = numpy.append(dataSet0[minTrainLen:len0, :], dataSet1[minTrainLen:len1, :], axis=0)
        y_test = numpy.append([True] * (len0 - minTrainLen), [False] * (len1 - minTrainLen))
    else:
        x_train = numpy.append(dataSet0[0:trainLen0, :], dataSet1[0:trainLen1, :], axis=0)
        y_train = numpy.append([True] * trainLen0, [False] * trainLen1)
        x_test = numpy.append(dataSet0[trainLen0:len0, :], dataSet1[trainLen1:len1, :], axis=0)
        y_test = numpy.append([True] * (len0 - trainLen0), [False] * (len1 - trainLen1))
    return x_train, y_train, x_test, y_test

param_path_0	  = "0.bin"	#файл с примерами 0 
param_path_1 	  = "1.bin"	#файл с примерами 1 
param_path_signal = "F:/ST_Opora/База данных/1_августа/bin/test_trace_1/C1_stray.bin"	#файл сигнала
param_split		  = 0.75		#часть на тестовую выборку
param_cross 	  = False		#допускается смешивание тестовой и обучающей выборки
param_sig_part    = 10000		#проверяемая на 1 часть сигнала
param_epoch       = 10 		#число эпох обучения
param_test        = True
param_find_signal = True

signal_1 	= readbin(param_path_1)
signal_0	= readbin(param_path_0)

param_dim   = signal_1.shape[1] #1483, длина примера (фрейма)

x_train, y_train, x_test, y_test = prepDataSet(signal_1, signal_0, param_split, param_split, param_cross)
del signal_1, signal_0

x_train = x_train.astype("float32")
x_test  = x_test.astype("float32")
y_train = y_train.astype("bool")
y_test  = y_test.astype("bool")

print("x_train shape:", x_train.shape)
print("y_train shape:", y_train.shape)
print("x_test shape:", x_test.shape)
print("y_test shape:", y_test.shape)

model = Sequential()
model.add(Dense(8000, input_dim=param_dim, activation="relu", kernel_initializer="normal"))
model.add(Dense(1, activation="sigmoid", kernel_initializer="normal"))
model.compile(loss="binary_crossentropy", optimizer="SGD", metrics=["accuracy"])
model.fit(x_train, y_train, batch_size=100, epochs=param_epoch, validation_split=0.1, verbose=2)

del x_train, y_train

#Оценить качество обучения на тестовой выборке
if param_test:
    scores = model.evaluate(x_test, y_test, verbose=2)
    print(": %.2f%%" % (scores[1]*100))
del x_test, y_test

#Предсказать позиции начала единичных кадров (макс. отклик)
if param_find_signal:
    signal = readSignal(param_path_signal) 					#загрузить из файла
    print(signal.shape)
    frames = getFrames(signal[0:param_sig_part], param_dim, 1, [0])	#взять первые отсчеты и создать набор примеров с шагом 1
    del signal
    predicts = model.predict(frames)
    plt.plot(predicts)
    plt.show()
    del frames

print "end script"

