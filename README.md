# Magnum opus 2022-2023

### Joey de Waal

#### S143533

## Documentatie

| opties   | Beschrijving                                                                                                                                                                                                                                                                    |
| -------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| api_key  | **Type**: String <br /> **Uitleg**: Hier vul je de API-sleutel in van Polygon.io. De use case stuurt hier verzoeken naar om de prijs op te halen.                                                                                                                               |
| api_port | **Type**: String <br /> **Uitleg**: De service luistert op deze poort. Wanneer je wilt inloggen om de grafieken te kunnen zien moet je naar deze poort surfen.<br /> Merk op dat het type een String is maar er nog steeds een getal moet staan in de string. Bv: “80”, “8080”. |

## Voorbeeld

Een voorbeeld van een minimale playbook ziet er als volgt uit.

```yml
---
- hosts: joey
  roles:
    - role: IAC-project
      vars:
        api_key: "ziUzGXKcxEWqbFO8GCYgKf6psfwbD9cs" # <- geen echte sleutel
        api_port: "8081"
```
