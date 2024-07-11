Enumerate all api sets of the current 64bit (x86_64, arm64) windows 10\11 machine. <br /><br />
A csv of the following format will be created: <br />
| ApiSet  | Modules |
| ------------- | ------------- |
| api-ms-win-core-io-l1-1-1  | kernel32.dll  |
| api-ms-win-...  | mod1.dll,mod2.dll  |

> [!NOTE]
> Has shown in the table, when two or more modules are used to implement an API set; Both will be written to the corresponding "modules" cell. <br /> (as "mod1.dll,mod2.dll" in the .csv)
