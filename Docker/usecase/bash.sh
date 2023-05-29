docker build -t usecase .
docker tag usecase joeydewaal/usecase
docker push joeydewaal/usecase
clear
docker compose up
