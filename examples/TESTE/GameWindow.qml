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
 * @author Rodrigo Goncalves de Oliveira <rodrigo.goncalves@openbossa.org>
 * @author Roger Felipe Zanoni da Silva <roger.zanoni@openbossa.org>
 */

QuasiGame {
    id: game

    width: 400
    height: 250

    currentScene: scene

    QuasiScriptBehavior {
        id: sideScrollBehavior

        script: {
            var newPos = entity.x + entity.scrollStep
            entity.x = newPos > scene.width ? 0 : newPos
            entity.x = entity.x < 0 ? scene.width : entity.x
        }
    }

    QuasiScriptBehavior {
        id: verticalScrollBehavior

        script: {
            var newPos = entity.y + entity.scrollStep
            entity.y = newPos > scene.height ? 0 : newPos
            entity.y = entity.y < 0 ? scene.height : entity.y
        }
    }

    QuasiScene {
        id: scene

        width: parent.width
        height: parent.height
    }
}