# Raw Measurement Data

`results.csv.gz.b64.part00` ... `results.csv.gz.b64.part04` contain a compressed copy of `../results.csv`.

Restore it with:

```bash
cat results.csv.gz.b64.part* | base64 -d | gzip -dc > ../results.csv
```
