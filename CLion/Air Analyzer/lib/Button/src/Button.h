 /**
  * @brief This library allows to manage a button.
  * It can specify if the pressure is long or not. Morevoer, is possible to assign a time (in milliseconds) to consider the long press, 
  * and a specific procedure both for short and long press. For short press there is the debouncing.
  * Copyright (c) 2022 Davide Palladino.
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact davidepalladino@hotmail.com
  * @website https://davidepalladino.github.io/
  * @version 2.0.2
  * @date 30th September, 2022
  * 
  */

#ifndef BUTTON_H
    #define BUTTON_H
    
    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    #define DEFAULT_LONG_PRESS 0                                                                // Default value in milliseconds for the long press.

    typedef enum input : uint8_t {B_PULLUP, B_NOPULLUP} input_t;                                // Symbolic constants to indicate, respectively, if is "INPUT_PULLUP" or "INPUT".
    typedef enum longPress : uint8_t {B_CONTINUOUS, B_NOTCONTINUOUS} longPress_t;               // Symbolic constants to indicate if the long press will be considered continuous or not.

    /**
     *  @brief Poiter type to a procdeure, that will be assigned to the member data "ptrActionShort" and "ptrActionLong" through the costructor or
     *  the relative methods.
     *  @warning: The procedures must be without parameters.
     */
    typedef void (*ptrProcedure) ();

    class Button {
        public:            
            /** 
             * @brief This constructor creates the object setting only the pin button. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @warning The pin will be set to "B_NOPULLUP" mode, respectively "INPUT" of "pinMode".
             */
            Button(uint8_t pin);

            /** 
             * @brief This constructor creates the object setting the pin button and the mode of the input. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param mode Mode of the input, between "INPUT" (with "B_NOPULLUP" constant) and "INPUT_PULLUP" (with "B_PULLUP" constant).
             */
            Button(uint8_t pin, input_t mode);

            /** 
             * @brief This constructor creates the object setting the pin button and the time to define the long press. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param timeLongPress Time in milliseconds for long press.
             * @warning The pin will be set to "B_NOPULLUP" mode, respectively "INPUT" of "pinMode". The type of long press will be set to "B_NOTCONTINUOUS";
             */
            Button(uint8_t pin, uint32_t timeLongPress);

            /** 
             * @brief This constructor creates the object setting the pin button and the time to define the long press. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param timeLongPress Time in milliseconds for long press.
             * @param typeLongPress Type of long press, if continuous (with "B_CONTINUOUS" constant) or not (with "B_NOTCONTINUOUS" constant).
             * @warning The pin will be set to "B_NOPULLUP" mode, respectively "INPUT" of "pinMode".
             */
            Button(uint8_t pin, uint32_t timeLongPress, longPress_t typeLongPress);

            /** 
             * @brief This constructor creates the object setting the pin button, the mode of the input and the time to define the long press. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param mode Mode of the input, between "INPUT" (with "B_NOPULLUP" constant) and "INPUT_PULLUP" (with "B_PULLUP" constant).
             * @param timeLongPress Time in milliseconds for long press.
             * @warning The type of long press will be set to "B_NOTCONTINUOUS".
             */
            Button(uint8_t pin, input_t mode, uint32_t timeLongPress);

            /** 
             * @brief This constructor creates the object setting the pin button, the mode of the input and the time to define the long press. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param mode Mode of the input, between "INPUT" (with "B_NOPULLUP" constant) and "INPUT_PULLUP" (with "B_PULLUP" constant).
             * @param timeLongPress Time in milliseconds for long press.
             * @param typeLongPress Type of long press, if continuous (with "B_CONTINUOUS" constant) or not (with "B_NOTCONTINUOUS"" constant).
             */
            Button(uint8_t pin, input_t mode, uint32_t timeLongPress, longPress_t typeLongPress);

            /** 
             * @brief This constructor creates the object setting the pin button, the time to define the long press and the procedures for short and long press. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param timeLongPress Time in milliseconds for long press.
             * @param ptrActionShort Pointer to a procedure for the short press. The procedure must be without parameters.
             * @param ptrActionLong Pointer to a procedure for the long press. The procedure must be without parameters.
             * @warning The pin will be set to "B_NOPULLUP" mode, respectively "INPUT" of "pinMode". The type of long press will be set to "B_NOTCONTINUOUS";
             */
            Button(uint8_t pin, uint32_t timeLongPress, ptrProcedure ptrActionShort, ptrProcedure ptrActionLong);

            /** 
             * @brief This constructor creates the object setting the pin button, the time to define the long press and the procedures for short and long press. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param timeLongPress Time in milliseconds for long press.
             * @param typeLongPress Type of long press, if continuous (with "B_CONTINUOUS" constant) or not (with "B_NOTCONTINUOUS" constant).
             * @param ptrActionShort Pointer to a procedure for the short press. The procedure must be without parameters.
             * @param ptrActionLong Pointer to a procedure for the long press. The procedure must be without parameters.
             * @warning The pin will be set to "B_NOPULLUP" mode, respectively "INPUT" of "pinMode".
             */
            Button(uint8_t pin, uint32_t timeLongPress, longPress_t typeLongPress, ptrProcedure ptrActionShort, ptrProcedure ptrActionLong);

            /** 
             * @brief This constructor creates the object setting the pin button, the mode of the input, the time to define the long press and the procedures for short and long press. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param mode Mode of the input, between "INPUT" (with "B_NOPULLUP" constant) and "INPUT_PULLUP" (with "B_PULLUP" constant).
             * @param timeLongPress Time in milliseconds for long press.
             * @param ptrActionShort Pointer to a procedure for the short press. The procedure must be without parameters.
             * @param ptrActionLong Pointer to a procedure for the long press. The procedure must be without parameters.
             * @warning The type of long press will be set to "B_NOTCONTINUOUS".
             */
            Button(uint8_t pin, input_t mode, uint32_t timeLongPress, ptrProcedure ptrActionShort, ptrProcedure ptrActionLong);

            /** 
             * @brief This constructor creates the object setting the pin button, the mode of the input, the time to define the long press and the procedures for short and long press. Moreover, it calls "pinMode".
             * @param pin Digital pin of the button.
             * @param mode Mode of the input, between "INPUT" (with "B_NOPULLUP" constant) and "INPUT_PULLUP" (with "B_PULLUP" constant).
             * @param timeLongPress Time in milliseconds for long press.
             * @param typeLongPress Type of long press, if continuous (with "B_CONTINUOUS" constant) or not (with "B_NOTCONTINUOUS constant).
             * @param ptrActionShort Pointer to a procedure for the short press. The procedure must be without parameters.
             * @param ptrActionLong Pointer to a procedure for the long press. The procedure must be without parameters.
             */
            Button(uint8_t pin, input_t mode, uint32_t timeLongPress, longPress_t typeLongPress, ptrProcedure ptrActionShort, ptrProcedure ptrActionLong);

            /**
             * @brief This method sets the time for the long press.
             * @param timeLongPress Time in milliseconds for long press.
             */
            void setTimeLongPress(uint32_t timeLongPress);

            /**
             * @brief This method gets the time for the long press.
             * @return Time in milliseconds for long press.
             */          
            uint32_t getTimeLongPress();

            /**
             * @brief This method sets the type of long press, if continuous or not.
             * @param typeLongPress Value between "B_CONTINUOUS" and "B_NOTCONTINUOUS".
             */
            void setTypeLongPress(longPress_t typeLongPress);

            /**
             * @brief This method gets the type of long press, if continuous or not.
             * @return Value between "B_CONTINUOUS" and "B_NOTCONTINUOUS".
             */
            longPress_t getTypeLongPress();

            /**
             * @brief This method sets the pointer to the procedure, for the short press.
             * @param ptrActionShort Pointer to a procedure for the short press. The procedure must be without parameters.
             */            
            void setPtrActionShort(ptrProcedure ptrActionShort);

            /**
             * @brief This method sets the pointer to the procedure, for the long press.
             * @param ptrActionLong Pointer to a procedure for the long press. The procedure must be without parameters.
             */   
            void setPtrActionLong(ptrProcedure ptrActionLong);          

            /**
             * @brief This method gets the actual press, both for short and long. For short press there is the debouncing.
             * @return Value -1 if the pressure is long; 0 if there is not any pressure; 1 if the pressure is short.
             */     
            int8_t checkPress();

        private:
            uint8_t pin;                                // Pin of the button to read the status.
            uint8_t mode;                               // Mode of the input, between "INPUT" (with "B_NOPULLUP" constant) and "INPUT_PULLUP" (with "B_PULLUP" constant).
            uint32_t timeLongPress;                     // Time in milliseconds for the long press.
            longPress_t typeLongPress;                  // Type of long press, if continuous (with "B_CONTINUOUS" constant) or not (with "B_NOTCONTINUOUS" constant).
            ptrProcedure ptrActionLong;                 // Pointer to a procedure for short press.
            ptrProcedure ptrActionShort;                // Pointer to a procedure for long press.
            uint8_t valuePress;                         // This variable will contain the value where the button will be considered pressed. In example, "HIGH" if the "pinMode" is set to "INPUT"; "LOW" if the "pinMode" is set to "INPUT_PULLUP".
            uint32_t timeOut;                           // End of time, calculated with the actual time plus "TIME_LONG_PRESS".
            bool isPressed;                             // Flag to indicate if the button was already pressed.
            bool isLongPressed;                         // Flag to indicate if the last press was long or not.
            int8_t actualValue;                         // Actual value of the button.

            /**
             * @brief This method sets the input mode.
             * @param mode Mode of the input.
             */
            void setMode(input_t mode);

            /**
             * @brief This method sets the value where the button will be considered pressed.
             */
            void setValuePress();  
    };
#endif
