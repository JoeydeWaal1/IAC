# **Magnum opus 2022-2023**

### Joey de Waal

#### S143533 2ITCSC1

<br />

## **Quickstart**

Een voorbeeld van een minimale playbook ziet er als volgt uit.

```yml
---
- hosts: joey
  roles:
    - role: IAC-project
      vars:
        api_key: "ziUzGXKcxEqebFOwGC8gKf6psfwbVasa" # <- geen echte sleutel
        api_port: "8081"
```

## **Documentatie**

| opties       | Beschrijving                                                                                                                                                                                                                                                                    |
| ------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **api_key**  | **Type**: String <br /> **Uitleg**: Hier vul je de API-sleutel in van [Polygon.io](https://polygon.io/). De use case stuurt hier verzoeken naar om de prijs op te halen.                                                                                                        |
| **api_port** | **Type**: String <br /> **Uitleg**: De service luistert op deze poort. Wanneer je wilt inloggen om de grafieken te kunnen <br />zien moet je naar deze poort surfen. Merk op dat het type een String is maar er nog steeds een getal moet staan in de string. Bv: “80”, “8080”. |

<br />

### **Dependencies**

- Internet verbinding
- docker

* Redhat:

  - epel
  - yum-utils
  - python3-pip
  - python3
  - pip: docker-py

* Debian:

  - pip
  - pip: docker

  <br />

### **Werking**

Tijdens het uitvoeren van mijn role via een playbook, word er als aller eerste gekeken naar de OS-family. Op basis hiervan worden er extra commando's uitgevoerd om de dependancies te installeren. De role configureerd eerst de Firewall en dan de dependancies. Nu word er een docker netwerk aangemaakt. Via mijn [dockerhub](https://hub.docker.com/) worden er 2 images via docker gedownload. Deze containers worden dan gestart.

<br />

### **Compatible**

| **O.S.**    | **Versie**    |
| ----------- | ------------- |
| Rocky Linux | 8.8           |
| Debian      | 11 (bullseye) |
