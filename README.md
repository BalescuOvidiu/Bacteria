# Bacteria

<table border="0" bordercolor="#FFFFFF">
  <tr border="0" bordercolor="#FFFFFF">
    <td border="0" bordercolor="#FFFFFF" valign="top" width="70%">
      <p>
        An Arduino 8x8 matrix game for Introduction to robotics, a turn-based role-play game.
      </p>
      <p>
        The game is runs on a 20x20 square matrix. On some squares there are bacteria. A bacterium is bordered by up to 8. If there are two bacteria around a square, a bacterium will appear in the next generation. If more than 4 bacteria are found around a bacterium, then it will die.
      </p>
    </td>
    <td border="0" bordercolor="#FFFFFF">
      <img src="https://raw.githubusercontent.com/BalescuOvidiu/Bacteria/master/img/photo.jpg"/>
    </td>
  </tr>
  <tr>
    <td colspan="2">
      <h3>Gameplay</h3>
      <p>
        In the menu, a DNA section is displayed on the LED matrix. When the player moves or press the joystick, or press button, then the game starts. The player is a bactera and can move on the matrix using the joystick.
      </p>
      <p>
        If joystick is pressed or moved and player is not bordered by more 4 bacteria, then the game will go to next generation(<b>level</b>). If player is bordered by more 4 bacteria, then game will go to restart room. In restart room, on matrix is a X and on LCD is listed the <b>score</b>, max number of generation survived by player. 
        <a href="https://drive.google.com/open?id=1Pe8iDwB8iUbNWSNGQlBPPpknMyZsimqw">Link</a> to the video.
      </p>
    </td>
  <h3>Electronic</h3>
  </tr>
  <tr border="0" bordercolor="#FFFFFF">
    <td border="0" bordercolor="#FFFFFF" valign="top" width="30%">
      <ol>
        <li>Arduino UNO R3 board</li>
        <li>Mini Breadboard</li>
        <li>Breadboard HQ 400 points</li>
        <li>10k ohm potentiometer</li>
        <li>10 microfarads capacitor</li>
        <li>104 picofarads capacitor</li>
        <li>10k ohm resistor</li>
        <li>220 ohm resistor</li>
        <li>Max7219 driver</li>
        <li>LED matrix 8x8 1088AS</li>
        <li>LCD 1602 Module</li>
        <li>Joystick</li>
        <li>Button</li>
      </ol>
    </td border="0" valign="middle">
    <td border="0">
      <img src="https://raw.githubusercontent.com/BalescuOvidiu/Bacteria/master/img/schematic.png"/>
    </td>
  </tr>
</table>
