# Magic-Spellbook
Arduino code for a magic spell book with reed switches that lit an LED jewell in the center and unlocked inner compartment at the correct combination.  One version opened a secret door. 
  /*WORKING version where: 
 * reed switches light up One center RBG LED using a protoshield
 * debouncing working
 * Muliple wands fixed
 * maglock good
 
 * To do:  
 * - Set up a reset switch/system (reed switch so when box closed auto-reset)
 * - Design contingency for battery dead
 * - begin world domination plan XK-1
 * - Moo hah haaaaaaa!!!!
 */
 
/*REED SWITCHE WIRING:  
 * wire 5v to one end of reed switch
 * Other end wire to pin #X (4,12,11,7) AND THEN 10K Ohm resister and THEN resister to gnd 
  (has to be in THAT order to work)
  
 LED WIRING
 * USE Radio Shack common-anode RGB part number 2760028
 * If the longest wire is 2nd from the left that is the anode and the other 3 from left to right
 * are red, blue, green (not RGB -> RBG).  
 * Wire the anode (long wire) to 5v power
 * left pin (red) 150 Ohm resistor then to A3
 * Third pin from left (the one to right of long pin - blue) 75 Ohm resistor and then to A1
 * Right-most pin (green) 75 Ohm resistor and then to A2
 */
