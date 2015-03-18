# Context #

This is the project for the course "Foundations of Computer Graphics" at [Instituto de Informática](http://www.inf.ufrgs.br/), [UFRGS](http://www.ufrgs.br/ufrgs/inicial), Brazil.

| **Course:** | INF01047 - Foundations of Computer Graphics |
|:------------|:--------------------------------------------|
| **Semester:** | 2011/1 |
| **Professors:** | [Marcelo Walter](http://www.inf.ufrgs.br/index.php?option=com_content&view=article&id=515&catid=39%3Acorpo-docente&Itemid=81), [Carla Freitas](http://inf.ufrgs.br/index.php?option=com_content&view=article&id=67%3Acarla-maria-dal-sasso-freitas&catid=39%3Acorpo-docente&Itemid=81) |
| **Students:** | Éderson Vieira, [Lucas Lersch](http://www.inf.ufrgs.br/~lslersch/) |

# Description #

This project was developed in the C programming language, using tools offered by the OpenGL API. Employing these tools, we used illumination, textures and geometric objects. We imported objects and models using the GLM library. We got the objects and models from the Google 3D Warehouse (Google SketchUp) in the Collada format and they were converted for the .obj format using 3D Studio Max.

For drawing the scene, 3D models were imported and/or modeled by us (in the case of simple geometric objects).

Game physics is implemented, such as gravity and friction with the ground. Physic elements are present to achieve a certain level of realism, such as minimal required velocity for the airplane to take off and be stable during the flight. Collision against the ground is also implemented.

Four types of cameras are implemented: first person, orthographic, third person and a camera that follows the airplane from the above.

The plane has three basic movements (Yaw, Pitch and Roll) and can accelerate and decelerate.

# Screenshots #
|<img src='http://www.inf.ufrgs.br/~lslersch/lib/exe/fetch.php?t=1409768090&w=498&h=500&tok=8b675e&media=fs_screenshot001.jpg' height='256' width='256'><table><thead><th><img src='http://www.inf.ufrgs.br/~lslersch/lib/exe/fetch.php?t=1409768091&w=499&h=500&tok=c819c7&media=fs_screenshot002.jpg' height='256' width='256'></th><th><img src='http://www.inf.ufrgs.br/~lslersch/lib/exe/fetch.php?t=1409768091&w=500&h=500&tok=1d7879&media=fs_screenshot003.jpg' height='256' width='256'></th></thead><tbody>
<tr><td>First person camera.</td><td>Third person camera.</td><td>View of plane in scenario.</td></tr></tbody></table>

<h1>Commands</h1>

<table><thead><th><img src='http://www.inf.ufrgs.br/~lslersch/lib/exe/fetch.php?t=1409768090&w=500&h=330&tok=db5082&media=airplane_movs.png'></th></thead><tbody></tbody></table>

<b>Airplane Controls:</b>

'+' - Accelerate the airplane<br>
<br>
'-' - Decelerate the airplane<br>
<br>
'W' - Pitch (-)<br>
<br>
'S' - Pitch (+)<br>
<br>
'A' - Yaw (-)<br>
<br>
'D' - Yaw (+)<br>
<br>
'Q' - Roll (-)<br>
<br>
'E' - Roll (+)<br>
<br>
<b>Cameras:</b>

'F1' - First Person Camera<br>
<br>
'F2' - Orthographic Camera<br>
<br>
'F3' - Third Person Camera<br>
<br>
'F4' - Above Camera<br>
<br>
'ESC' - Exit the game<br>
<br>
<h1>Instructions</h1>
<ul><li>There is collision against the ground.<br>
</li><li>If the airplane is touching the ground, there is friction.<br>
</li><li>The airplane has a minimum required velocity to take off.<br>
</li><li>The gravity affects the airplane if the velocity is lower than the minimum required velocity.<br>
</li><li>The airplane only moves if it has velocity.</li></ul>

<h1>References</h1>
<b>GLM Library</b> - <a href='http://www.3dcodingtutorial.com/Working-with-3D-models/Getting-GLM.html'>http://www.3dcodingtutorial.com/Working-with-3D-models/Getting-GLM.html</a>

<b>Google Sketchup</b> - <a href='http://sketchup.google.com'>http://sketchup.google.com</a>

<b>3D studio Max</b> - <a href='http://usa.autodesk.com/3ds-max/'>http://usa.autodesk.com/3ds-max/</a>