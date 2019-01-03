using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Linq;

public class MathHelper
{
    //Formulas
    public static float Randomizer(float value)             //Random value from -r to r
    {
        value = UnityEngine.Random.Range(-value, value);    //Unity Random function takes floats
        return value;
    }
    //-- ACTIVATION FUNCTION TYPES --
    public static double Sigmoid(double value)      //Non-linear
    {                                               //Values range between 0 and 1
        double k = Math.Exp(value);                 //Better used in output layer for binary classification
        return k / (1.0f + k);
    }

    public static double TanH(double value)         //Non-linear
    {                                               //Values range between -1 and 1
        double k = Math.Exp(-2 * value);            //Usually for hidden layers since the values lie between -1 abnd 1
        return 2 / (1.0f + k) - 1;                  //The mean for the hidden layer comes out be 0 or very close to it, helps centering data
    }

    public static double ReLu(double value)         //Non-linear, can easily backpropagte errors
    {                                               //Value ranges between 0 and INF
        return Math.Max(0, value);                  //Less computationally expensive than Sigmoid or TanH
    }

    public static double LeakyReLu(double value)
    {
        if (value < 0) { return 0.01 * value; }
        else return value;
    }
}
