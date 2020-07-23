import pathlib

import markdown


root = pathlib.Path('.')

for project in root.iterdir():
    if not project.is_dir():
        continue
    readme = project / 'README.md'
    if not readme.exists():
        continue
    with open(readme) as f:
        content = f.read()
    html = markdown.markdown(content)
    with open(project / 'DESCRIPTION', 'w') as f:
        f.write(html)
