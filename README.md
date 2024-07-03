Enumerate all api sets of the current 64bit (x86_64, arm64) windows 10\11 machine. <br /><br />
A csv of he following format will be created: <br />
| api set name  | module name |
| ------------- | ------------- |
| api-ms-win-core-io-l1-1-1  | kernel32.dll  |
| api-ms-win-...  | mod1.dll,mod2.dll  |

> [!NOTE]
> Has shown in the table, when two or more modules are used to implement the same api set; They will be in the same cell <br /> (in the csv file represented as "mod1.dll,mod2.dll")
