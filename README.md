# Bacteria

<table border="0" bordercolor="#FFFFFF">
  <tr border="0" bordercolor="#FFFFFF">
    <td border="0" bordercolor="#FFFFFF" valign="top" width="70%">
      <p>
        An Arduino 8x8 matrix game for Introduction to robotics, a turn-based role-play game.
      </p>
      <p>
        The game is runs on a 20x20 square matrix. On some squares there are bacteria. A bacteria is bordered by up to 8. If there are two bacteria around a square, a bacteria will appear in the next generation. If more than 4 bacteria are found around a bacteria, then it will die.
      </p>
    </td>
    <td border="0" bordercolor="#FFFFFF">
      <img src="https://raw.githubusercontent.com/BalescuOvidiu/Bacteria/master/img/photo.jpg"/>
    </td>
  </tr>
</table>

<h3>Gameplay</h3>
<p>
  The player is a bacterium and can move on the matrix using the joystick. If joystick is pressed or moved and player is not bordered by more 4 bacteria, then the game will go to next generation(<b>level</b>).
</p>
<p>
  If player is bordered by more 4 bacteria, then game will go to restart room. In restart room, on matrix is a X and on LCD is listed the <b>score</b>, max number of generation survived by player.  
</p>

<h3>Electronic</h3>
<table border="0" bordercolor="#FFFFFF">
  <tr border="0" bordercolor="#FFFFFF">
    <td border="0" bordercolor="#FFFFFF" valign="top" width="30%">
      <ol>
        <li>Arduino Uno Board</li>
        <li>Small breadboard</li>
        <li>Medium breadboard</li>
        <li>Potentiometer</li>
        <li>10 microfarad capacitor</li>
        <li>104 microfarad capacitor</li>
        <li>10k ohm resistor</li>
        <li>220 ohm resistor</li>
        <li>Max7219 driver</li>
        <li>LED matrix 8x8 1088AS</li>
        <li>LCD display</li>
        <li>Joystick</li>
      </ol>
    </td border="0" valign="middle">
    <td border="0">
      <img src="https://raw.githubusercontent.com/BalescuOvidiu/Bacteria/master/img/schematic.png"/>
    </td>
  </tr>
</table>
