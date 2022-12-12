# how to develop web ui

## copy files except config.html to static/

```
cd embed
rsync -v --exclude=config.html ./* static
```

## run python http.server or some simple web server on embed

```
cd embed
python -m http.server
```

## Access web via web browser

Use firefox or chrome newer version.

# after development

Dont forget to write back all files in static/ to embed.

```
cd embed/static
cp -v ./* ../
```