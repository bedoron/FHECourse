For help run:
```make```

Build docker image:
```make image```

Start docker image in the background:
```make start```
This will start the image with src mounted into the container.

Log into it to look around:
```make bash```
This will put you where the actual makefile is, hit make to rebuild (remember that src folder is mounted)

If you've made changes to the code and you want to run it:
```make run```
This will also compile the code (this requires the container to be running)


