XD Dictionary
=================================================

XD is a simple and Cross-Platform dictionary software written in Qt4.

## Directory structure (Windows version)

```
│  xd.exe
│  index-creator.exe
│  msvcr80.dll
│  msvcp80.dll
│  QtCore4.dll
│  QtGui4.dll
│  Microsoft.VC80.CRT.manifest
│  options.ini
│  app.ini
│
├─doc
│  readme.txt
│  licence.txt
│
├─dict
│  top.index
│  bottom.index
│  middle.index
│  jp-jp_koujien.dict
│  jp-kr_kjdict.dict
│  kr-jp_kjdict.dict
│  en-ja_jmdict.dict
│  en-cn_jianming.dict
│  cn-en_jianming.dict
│  cn-cn_gaojihanyu.dict
│
└─lang
   zh_CN.qm
   en.qm
```

## Dictionary file format

```
Dict-Name: XXXX XXXX
Content-Type: text or html
Word-Count: 123456

keyword1|keyword2|keyword3...
[[pronunciation]]
Description

keyword1|keyword2|keyword3...
[[pronunciation]]
Description
```
