#!/usr/bin/env node

/**
 * MongoDB POC - Basic AREA operations
 */

const { MongoClient } = require('mongodb');

const MONGODB_URI = 'mongodb://localhost:27017';
const DB_NAME = 'area_poc';

async function runMongodbPOC() {

    const client = new MongoClient(MONGODB_URI);

    try {
        await client.connect();

        const db = client.db(DB_NAME);
        await db.dropDatabase();

        const users = db.collection('users');
        const user = await users.insertOne({
            email: 'test@area.com',
            name: 'John Doe',
            createdAt: new Date()
        });

        const services = db.collection('services');
        await services.insertMany([
            { name: 'gmail', displayName: 'Gmail' },
            { name: 'github', displayName: 'GitHub' }
        ]);

        const areas = db.collection('areas');
        const gmail = await services.findOne({ name: 'gmail' });
        const github = await services.findOne({ name: 'github' });

        const area = await areas.insertOne({
            userId: user.insertedId,
            name: 'Gmail to GitHub',
            actionService: gmail._id,
            reactionService: github._id,
            enabled: true,
            createdAt: new Date()
        });

        const userAreas = await areas.aggregate([
            { $match: { userId: user.insertedId } },
            {
                $lookup: {
                    from: 'services',
                    localField: 'actionService',
                    foreignField: '_id',
                    as: 'action'
                }
            }
        ]).toArray();


    } catch (error) {
        console.error('❌ Error:', error.message);
    } finally {
        await client.close();
    }
}

if (require.main === module) {
    runMongodbPOC().catch(console.error);
}

module.exports = { runMongodbPOC };