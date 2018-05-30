#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from ctypes import *
import chess
import numpy as np
import tensorflow as tf
import keras
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras import backend as K

batch_size = 128

model = Sequential()
model.add(Dense(units=512, activation="relu", input_dim=384))
model.add(Dense(units=512, activation="softmax"))

model.compile(loss="categorical_crossentropy",
              optimizer="sgd",
              metrics=["accuracy"])

history_callback = model.fit(x_train, y_train, epochs=5, batch_size=32)
loss_history = history_callback.history["loss"]

cell =

helper = tf.contrib.seq2seq.GreedyEmbeddingHelper(
    embedding=embedding,
    start_tokens=tf.tile([GO_SYMBOL], [batch_size]),
    end_token=END_SYMBOL)

decoder = tf.contrib.seq2seq.BasicDecoder(
    cell=cell,
    helper=helper,
    initial_state=cell.zero_state(batch_size, tf.float32))

outputs, _ = tf.contrib.seq2seq.dynamic_decode(
   decoder=decoder,
   output_time_major=False,
   impute_finished=True,
   maximum_iterations=220)


tf.logging.set_verbosity(tf.logging.INFO)


def train():
    board = chess.Board()
    model

#def main():
#    tf.app.run()
#
#if __name__ = "__main__":
#    main()
