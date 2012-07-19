/**
 * Copyright (C) 2012 by INdT
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * @author Sergio Correia <sergio.correia@openbossa.org>
 */

QuasiGame {
    id: game

    width: 1280
    height: 752

    currentScene: scene

    property int displacement: 10
    property int displacementThreshold: (game.displacement / 2) + 1

    function moveLeft() {
        console.log("moving left..")
        
        if (entity.x >= game.displacement) {
            entity.x -= game.displacement
        }
    }

    function moveRight() {
        console.log("moving right..")
        
        var planeX = entity.x + entity.width
        if (planeX <= game.width - game.displacement) {
	        entity.x += game.displacement
        }
    }

    function moveUp() {
        console.log("moving up..")
        if (entity.y >= game.displacement) {
            entity.y -= game.displacement
            entity.angle = 0
            entity.scale = 1.0
        }
    }

    function moveDown() {
        console.log("moving down..")
        var planeY = entity.y + entity.height
        if (planeY <= game.height - game.displacement) {
            entity.y += game.displacement
            entity.angle = 0
            entity.scale = 1.0
        }
    }

    QuasiScene {
        id: scene

        width: game.width
        height: game.height

        QuasiEntity {
            Image {
                id: background

                width: game.width
                height: game.height

                source: ":/images/bg.jpg"

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    
                    onPressed: {
                    	console.log("starting timer")
                    	timer.start()
                	}
                    onReleased: {
                    	console.log("stopping timer")
                    	timer.stop()
                	}
                }
            }
        }
        
        Timer {
            id: timer
            repeat: true
            interval: 10
            onTriggered: {
            	var planeX = entity.x + (entity.width / 2)
            	var diffX = Math.abs(game.mouse.x - planeX)
            	if (diffX >= game.displacementThreshold) {
	                if (game.mouse.x > planeX) {
	                    moveRight()
	                } else {
	                    moveLeft()
	                }
                }

				var planeY = entity.y + (entity.height / 2)
				var diffY = Math.abs(game.mouse.y - planeY)
				
                if (diffY >= game.displacementThreshold) {
	                if (game.mouse.y > planeY) {
	                    moveDown()
	                } else {
	                    moveUp()
	                }
                }
            }
        }

        QuasiEntity {
            Image {
                id: entity
                
                property real angle: 0

                width: 100
                height: 150

                smooth: true
                
                y: game.height - 2 * entity.height
                x: (game.width - entity.width) / 2

				property int rotationX: entity.x
                property int rotationY: entity.y
                
                Behavior on x {
                    SmoothedAnimation {}
                }
 
		        Behavior on y {
		            SmoothedAnimation {}
		        }

                source: ":/images/f35.png"
                transform: Rotation {
                	origin.x: entity.x
                	origin.y: entity.y
                	angle: entity.angle
                	axis { x: 0; y: 1; z: 0 }
                }
            }
        }

        focus: true
        Keys.onPressed: {
            switch (event.key) {
                case Qt.Key_Left: {
                    moveLeft()
                    break
                }
                
                case Qt.Key_Right: {
                    moveRight()
                    break
                }
                
                case Qt.Key_Up: {
                    moveUp()
                    break
                }
                
                case Qt.Key_Down: {
                    moveDown()
                    break
                }
                
                case Qt.Key_Escape: {
                	Qt.quit()
                }
            }
        }
    }
}

