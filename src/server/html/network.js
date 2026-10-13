const submitData = async (path, data) => {
    try {
        const res = await fetch(path, {
            method: "POST",
            body: JSON.stringify(data),
            headers: {
                "Content-type": "application/json",
            },
        });

        const reader = res.body.getReader();

        const rawData = [];
        let {done, value} = await reader.read();

        if (value != null) {
            rawData.push(value);
        }

        // TODO: This is causing issues with reading forever
        // while(!done) {
        //     ({ done, value } = await reader.read());
        //
        //     if(value != null)
        //         rawData.push(value);
        // }

        const parsedData = [];

        for (let i = 0; i < rawData.length; i++) {
            const chunkString = new TextDecoder().decode(rawData[i]);

            parsedData.push(JSON.parse(chunkString));
        }

        return parsedData;
    } catch (err) {
        console.error("Unable to fetch server data: ", err);
    }
}
