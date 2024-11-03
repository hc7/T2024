# Simulator 

## Architecture

Simulator has client - server architecture where server is a main component which works on client's requests of simulation project. 

Every simulation project consist of device configuration and logic. Every project keeps configuration in DB and project logic on file system. Every `Simulator Node` can provide any of base services : Database, Storage, Executor. It is possible to share working beatween all `Simulator Nodes` in net. First stage we implement only a node configuration with one database, one storage and one executor.

```mermaid
architecture-beta

    service user(mdi:human)[Client]
    group api(cloud)[Simulator Node]
    
    service server(server)[Server] in api

    service db(database)[Database] in api
    service disk1(disk)[Storage] in api
    service sim(mdi:desktop-tower)[Executor] in api

    user:R -- L:server

    server:R -- L:db
    server:R -- L:disk1
    server:R -- L:sim
    
```

## Object types


## Design

Simulation sequance for operating with project.

### General Sequance
```mermaid
sequenceDiagram
    actor User
    participant Server as Control
    participant fs
    participant db
    participant prj

    User->>+Server: +LOGIN
    Server->>-User: +LOGIN:OK    

    User->>+Server: CONFIGURATION:"project.config"
    
    Server->>Server: CONFIGURATION:check

    Server->>-User: +CONFIGURATION:OK

    Server->>+fs: open configuration file "project.config"
    fs->>-Server: ok
    Server->>User: +CONFIGURATION_FILE:OK

    Server->>+db: open db "project.config::db"
    db->>-Server: ok
    Server->>User: +CONFIGURATION_DB:OK

    Server->>+fs: open project file "project.config::project"
    fs->>-Server: ok
    Server->>User: +CONFIGURATION_PROJECT:OK

    Server->>+prj: instantiate project
    prj->>-Server: ok
    Server->>User: +PROJECT:OK

    loop While project going
        User-->>Server:Request
        Server-->>User:Answer

    end

    User->>+Server: +LOGOFF
    Server->>-User: +LOGOFF:OK
    
```
### Simulation Start Sequance

When simulation activated it is waiting for commands from `User` or from `Control`. Results 
transferring to `Control` for logging.
```mermaid
sequenceDiagram
    actor User
    participant Server as Control
    participant prj
    participant sim

    User->>+Server: +LOGIN
    Server->>-User: +LOGIN:OK    

    Server->>+prj: instantiate project
    prj->>+sim: create simulation obj (clone process)
    prj->>-Server: ok

    loop While simulation going (execute commands)
        Server -->>+sim: Command
        sim-->>-Server:Result
    end


    User->>+Server: +LOGOFF
    Server->>-User: +LOGOFF:OK

```
