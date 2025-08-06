; The Nullium Operating System
;    Copyright (C) 2025, jastahooman
;
;    This program is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation, either version 3 of the License, or
;    (at your option) any later version.
;
;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program.  If not, see <https://www.gnu.org/licenses/>.

; NASM assembly


gdtr DW 0 ; For limit storage
     DQ 0 ; For base storage

global setGdt

setGdt:
    MOV   [gdtr], DI
    MOV   [gdtr+2], RSI
    LGDT  [gdtr]
    RET